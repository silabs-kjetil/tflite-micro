/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <string.h>

#include "python/tflite_micro/python_ops_resolver.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub0_golden_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub0_input0_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub0_input1_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub0_model_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub1_golden_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub1_input0_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub1_input1_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub1_model_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub2_golden_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub2_input0_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub2_input1_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub2_model_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub3_golden_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub3_input0_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub3_input1_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub3_model_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub4_golden_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub4_input0_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub4_input1_int16_test_data.h"
#include "tensorflow/lite/micro/integration_tests/seanet/sub/sub4_model_data.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_allocator.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/micro/testing/micro_test.h"

constexpr size_t kTensorArenaSize = 1024 * 100;
uint8_t tensor_arena[kTensorArenaSize];

namespace tflite {
namespace micro {
namespace {

void RunModel(const uint8_t* model, const int16_t* input0,
              const uint32_t input0_size, const int16_t* input1,
              const uint32_t input1_size, const int16_t* golden,
              const uint32_t golden_size, const char* name) {
  InitializeTarget();
  MicroProfiler profiler;
  PythonOpsResolver op_resolver;

  MicroInterpreter interpreter(GetModel(model), op_resolver, tensor_arena,
                               kTensorArenaSize, nullptr, &profiler);
  interpreter.AllocateTensors();
  TfLiteTensor* input_tensor0 = interpreter.input(0);
  TF_LITE_MICRO_EXPECT_EQ(input_tensor0->bytes, input0_size * sizeof(int16_t));
  memcpy(interpreter.input(0)->data.raw, input0, input_tensor0->bytes);
  TfLiteTensor* input_tensor1 = interpreter.input(1);
  TF_LITE_MICRO_EXPECT_EQ(input_tensor1->bytes, input1_size * sizeof(int16_t));
  memcpy(interpreter.input(1)->data.raw, input1, input_tensor1->bytes);
  if (kTfLiteOk != interpreter.Invoke()) {
    TF_LITE_MICRO_EXPECT(false);
    return;
  }
  profiler.Log();
  MicroPrintf("");

  TfLiteTensor* output_tensor = interpreter.output(0);
  TF_LITE_MICRO_EXPECT_EQ(output_tensor->bytes, golden_size * sizeof(int16_t));
  int16_t* output = ::tflite::GetTensorData<int16_t>(output_tensor);
  for (uint32_t i = 0; i < golden_size; i++) {
    // TODO(b/205046520): Better understand why TfLite and TFLM can sometimes be
    // off by 1.
    TF_LITE_MICRO_EXPECT_NEAR(golden[i], output[i], 1);
  }
}

}  // namespace
}  // namespace micro
}  // namespace tflite

TF_LITE_MICRO_TESTS_BEGIN

TF_LITE_MICRO_TEST(sub0_test) {
  tflite::micro::RunModel(
      g_sub0_model_data, g_sub0_input0_int16_test_data,
      g_sub0_input0_int16_test_data_size, g_sub0_input1_int16_test_data,
      g_sub0_input1_int16_test_data_size, g_sub0_golden_int16_test_data,
      g_sub0_golden_int16_test_data_size, "sub0 test");
}

TF_LITE_MICRO_TEST(sub1_test) {
  tflite::micro::RunModel(
      g_sub1_model_data, g_sub1_input0_int16_test_data,
      g_sub1_input0_int16_test_data_size, g_sub1_input1_int16_test_data,
      g_sub1_input1_int16_test_data_size, g_sub1_golden_int16_test_data,
      g_sub1_golden_int16_test_data_size, "sub1 test");
}

TF_LITE_MICRO_TEST(sub2_test) {
  tflite::micro::RunModel(
      g_sub2_model_data, g_sub2_input0_int16_test_data,
      g_sub2_input0_int16_test_data_size, g_sub2_input1_int16_test_data,
      g_sub2_input1_int16_test_data_size, g_sub2_golden_int16_test_data,
      g_sub2_golden_int16_test_data_size, "sub2 test");
}

TF_LITE_MICRO_TEST(sub3_test) {
  tflite::micro::RunModel(
      g_sub3_model_data, g_sub3_input0_int16_test_data,
      g_sub3_input0_int16_test_data_size, g_sub3_input1_int16_test_data,
      g_sub3_input1_int16_test_data_size, g_sub3_golden_int16_test_data,
      g_sub3_golden_int16_test_data_size, "sub3 test");
}

TF_LITE_MICRO_TEST(sub4_test) {
  tflite::micro::RunModel(
      g_sub4_model_data, g_sub4_input0_int16_test_data,
      g_sub4_input0_int16_test_data_size, g_sub4_input1_int16_test_data,
      g_sub4_input1_int16_test_data_size, g_sub4_golden_int16_test_data,
      g_sub4_golden_int16_test_data_size, "sub4 test");
}

TF_LITE_MICRO_TESTS_END
