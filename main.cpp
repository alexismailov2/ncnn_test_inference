#include <iostream>

#include <ncnn/net.h>
#include <ncnn/layer.h>

//#include <opencv2/imgcodecs.hpp>

#include <fstream>

struct Object
{
  float x;
  float y;
  float w;
  float h;
  int label;
  float prob;
};

static ncnn::Mat RandomMat(int w, int h, int elempack)
{
  ncnn::Mat m(w, h, (size_t)elempack, elempack);

  unsigned char* p = m;
  for (int i = 0; i < w * h * elempack; i++)
  {
    p[i] = std::rand() % 256;
  }

  return m;
}

static int from_rgb(const unsigned char* rgb, int w, int h, int stride, ncnn::Mat& m)
{
  auto file = std::ofstream("image_array.bin", std::ios::binary);
  m.create(w, h, 3, 4u);
  if (m.empty())
    return -100;

  const int wgap = stride - w * 3;
  if (wgap == 0)
  {
    w = w * h;
    h = 1;
  }

  float* ptr0 = m.channel(0);
  float* ptr1 = m.channel(1);
  float* ptr2 = m.channel(2);

  for (int y = 0; y < h; y++)
  {
    int remain = w;

    for (; remain > 0; remain--)
    {
      *ptr0 = rgb[0];
      *ptr1 = rgb[1];
      *ptr2 = rgb[2];
      file.write((const char*)&rgb[0], 3);
      rgb += 3;
      ptr0++;
      ptr1++;
      ptr2++;
    }
    rgb += wgap;
  }
  return 0;
}

static int from_rgb2bgr(const unsigned char* rgb, int w, int h, int stride, ncnn::Mat& m)
{
  m.create(w, h, 3, 4u);
  if (m.empty())
    return -100;

  const int wgap = stride - w * 3;
  if (wgap == 0)
  {
    w = w * h;
    h = 1;
  }

  float* ptr0 = m.channel(0);
  float* ptr1 = m.channel(1);
  float* ptr2 = m.channel(2);

  for (int y = 0; y < h; y++)
  {
    int remain = w;

    for (; remain > 0; remain--)
    {
      *ptr0 = rgb[2];
      *ptr1 = rgb[1];
      *ptr2 = rgb[0];

      rgb += 3;
      ptr0++;
      ptr1++;
      ptr2++;
    }

    rgb += wgap;
  }

  return 0;
}

static int from_rgba2rgb(const unsigned char* rgba, int w, int h, int stride, ncnn::Mat& m)
{
  m.create(w, h, 3, 4u);
  if (m.empty())
    return -100;

  const int wgap = stride - w * 4;
  if (wgap == 0)
  {
    w = w * h;
    h = 1;
  }

  float* ptr0 = m.channel(0);
  float* ptr1 = m.channel(1);
  float* ptr2 = m.channel(2);

  for (int y = 0; y < h; y++)
  {
    int remain = w;
    for (; remain > 0; remain--)
    {
      *ptr0 = rgba[0];
      *ptr1 = rgba[1];
      *ptr2 = rgba[2];

      rgba += 4;
      ptr0++;
      ptr1++;
      ptr2++;
    }
    rgba += wgap;
  }
  return 0;
}

static ncnn::Net* yolo = 0;
int main()
{
  //cv::Mat inin = cv::imread("/Users/alexanderismailov/CLionProjects/ncnn_test_inference/1.bmp", cv::IMREAD_COLOR);

  char* rgba_data = new char[320*320*3];
  std::ifstream file("./assets/image_array.bin", std::ios::binary);
  file.read(rgba_data, 320*320*3);
  if (!yolo)
  {
    yolo = new ncnn::Net;
    yolo->load_param("./assets/yolo-fastest-opt.param");
    yolo->load_model("./assets/yolo-fastest-opt.bin");
  }

  const int target_size = 320;

  // letterbox pad to multiple of 32
  //char* rgba_data = new char[512*512*4*4];
  uint32_t width = 320;
  uint32_t height = 320;
  int w = width;
  int h = height;
  float scale = 1.f;
  if (w > h)
  {
    scale = (float)target_size / w;
    w = target_size;
    h = h * scale;
  }
  else
  {
    scale = (float)target_size / h;
    h = target_size;
    w = w * scale;
  }

  ncnn::Mat in;
  //from_rgba2rgb((const unsigned char*)rgba_data, (int)w, (int)h, (int)w * 4, in);
  //from_rgb2bgr((const unsigned char*)rgba_data, (int)w, (int)h, (int)w * 3, in);
  //from_rgb((const unsigned char*)inin.data, (int)w, (int)h, (int)w * 3, in);
  from_rgb((const unsigned char*)rgba_data, (int)w, (int)h, (int)w * 3, in);
  // pad to target_size rectangle
  // yolo/utils/datasets.py letterbox
  int wpad = (w + 31) / 32 * 32 - w;
  int hpad = (h + 31) / 32 * 32 - h;
  ncnn::Mat in_pad;
  ncnn::copy_make_border(in, in_pad, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, ncnn::BORDER_CONSTANT, 114.f);

  const float norm_vals[3] = {1 / 255.f, 1 / 255.f, 1 / 255.f};
  in_pad.substract_mean_normalize(0, norm_vals);

  ncnn::Extractor ex = yolo->create_extractor();

  ex.input("data", in_pad);
  ncnn::Mat out;
  ex.extract("output", out);

  int count = out.h;

  //    // apply nms with nms_threshold
  //    std::vector<int> picked;
  //    nms_sorted_bboxes(proposals, picked, nms_threshold);

  std::vector<Object> objects;
  objects.resize(count);
  for (int i = 0; i < count; i++)
  {
    int label;
    float x1, y1, x2, y2, score;
    float pw,ph,cx,cy;
    const float* values = out.row(i);

    x1 = values[2] * width;
    y1 = values[3] * height;
    x2 = values[4] * width;
    y2 = values[5] * height;

    score = values[1];
    label = values[0];

    if(x1<0) x1=0;
    if(y1<0) y1=0;
    if(x2<0) x2=0;
    if(y2<0) y2=0;

    if(x1>width) x1=width;
    if(y1>height) y1=height;
    if(x2>width) x2=width;
    if(y2>height) y2=height;

    objects[i].label = label;
    objects[i].prob = score;
    objects[i].x = x1;
    objects[i].y = y1;
    objects[i].w = x2 - x1;
    objects[i].h = y2 - y1;

    fprintf(stderr, "label: %d, prob: %f, x: %f, y: %f, w: %f, h: %f", objects[i].label, objects[i].prob, objects[i].x, objects[i].y, objects[i].w, objects[i].h);
  }
  return 0;
}
