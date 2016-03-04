#include <nan.h>

#include "core.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Local;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_METHOD(nodeCoreInit) {
  const String::Utf8Value corePath(info[0]->ToString());
  coreInit(*corePath);
}

NAN_METHOD(nodeCoreLoadGame) {
  const String::Utf8Value romPath(info[0]->ToString());
  coreLoadGame(*romPath);
}

NAN_METHOD(nodeCoreUpdate) {
  coreUpdate();
}

NAN_METHOD(nodeCoreVideoData) {
  size_t width, height;
  const auto & videoBuf = coreVideoData(width, height);

  auto obj = Nan::New<Object>();
  obj->Set(Nan::New("width").ToLocalChecked(), Nan::New((uint32_t)width));
  obj->Set(Nan::New("height").ToLocalChecked(), Nan::New((uint32_t)height));

  // Buffer access
  // Local<Object> array = info[0]->ToObject();
  Nan::TypedArrayContents<uint8_t> typedArray(info[0]);
  memcpy(*typedArray, &videoBuf[0], videoBuf.size() * 4);

  info.GetReturnValue().Set(obj);
}

NAN_METHOD(nodeCoreVideoSize) {
  size_t width, height;
  coreVideoData(width, height);

  auto obj = Nan::New<Object>();
  obj->Set(Nan::New("width").ToLocalChecked(), Nan::New((uint32_t)width));
  obj->Set(Nan::New("height").ToLocalChecked(), Nan::New((uint32_t)height));

  info.GetReturnValue().Set(obj);
}

NAN_METHOD(nodeCoreAudioData) {
  const auto audioBuf = coreAudioData();
  info.GetReturnValue().Set(Nan::CopyBuffer((const char *)&audioBuf[0], audioBuf.size() * 2).ToLocalChecked());
}

NAN_METHOD(nodeCoreTimings) {
  double fps, audioSampleRate;
  coreTimings(fps, audioSampleRate);

  auto obj = Nan::New<Object>();
  obj->Set(Nan::New("fps").ToLocalChecked(), Nan::New(fps));
  obj->Set(Nan::New("audio_sample_rate").ToLocalChecked(), Nan::New(audioSampleRate));

  info.GetReturnValue().Set(obj);
}

NAN_METHOD(nodeCoreSettingsSet)
{
  const String::Utf8Value key(info[0]->ToString());
  const String::Utf8Value val(info[1]->ToString());
  coreSettingsSet(*key, *val);
}

//
// void nodeCoreSettingsDesc(const FunctionCallbackInfo<Value> & args)
// {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);
//
//   if (args.Length() != 0) {
//     isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
//     return;
//   }
//
//   const auto d = coreSettingsDesc();
//
//   Local<Object> obj = Object::New(isolate);
//
//   for (const auto & settingsDescEntry : d) {
//     Local<Array> choices = Array::New(isolate, settingsDescEntry.choices.size());
//     for (size_t i=0; i<settingsDescEntry.choices.size(); i++) {
//       choices->Set(i, String::NewFromUtf8(isolate, settingsDescEntry.choices[i].c_str()));
//     }
//
//     Local<Object> objEntry = Object::New(isolate);
//     objEntry->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, settingsDescEntry.name.c_str()));
//     objEntry->Set(String::NewFromUtf8(isolate, "choices"), choices);
//
//     obj->Set(String::NewFromUtf8(isolate, settingsDescEntry.key.c_str()), objEntry);
//   }
//
//   args.GetReturnValue().Set(obj);
// }
//
// void nodeCoreJoypadDesc(const FunctionCallbackInfo<Value> & args)
// {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);
//
//   if (args.Length() != 0) {
//     isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
//     return;
//   }
//
//   Local<Array> res = Array::New(isolate, coreJoypadDesc().size());
//
//   size_t i = 0;
//   for (const auto & name : coreJoypadDesc()) {
//     res->Set(i++, String::NewFromUtf8(isolate, name.c_str()));
//   }
//
//   args.GetReturnValue().Set(res);
// }
//
// void nodeCoreJoypadPress(const FunctionCallbackInfo<Value> & args)
// {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);
//
//   if (args.Length() != 1) {
//     isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
//     return;
//   }
//
//   v8::String::Utf8Value name(args[0]->ToString());
//   coreJoypadPress(*name);
// }
//
// void nodeCoreJoypadRelease(const FunctionCallbackInfo<Value> & args)
// {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);
//
//   if (args.Length() != 1) {
//     isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
//     return;
//   }
//
//   v8::String::Utf8Value name(args[0]->ToString());
//   coreJoypadRelease(*name);
// }
//
// // @return Buffer containing the saved state
// void nodeCoreSaveState(const FunctionCallbackInfo<Value> & args)
// {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);
//
//   if (args.Length() != 0) {
//     isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
//     return;
//   }
//
//   const auto stateBuf = coreSaveState();
//
//   auto slowBuffer = Buffer::New(stateBuf.size());
//   memcpy(Buffer::Data(slowBuffer), &stateBuf[0], stateBuf.size());
//   args.GetReturnValue().Set(slowBuffer);
// }
//
//
// // @arg Buffer containing the state to restore
// void nodeCoreRestoreState(const FunctionCallbackInfo<Value> & args)
// {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);
//
//   if (args.Length() != 1) {
//     isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
//     return;
//   }
//
//   Local<Object> bufferObj = args[0]->ToObject();
//   char*  bufferData = Buffer::Data(bufferObj);
//   size_t bufferLength = Buffer::Length(bufferObj);
//   coreRestoreState(bufferData, bufferLength);
// }

NAN_MODULE_INIT(init) {
  Set(target, New("coreInit").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreInit)).ToLocalChecked());
  Set(target, New("coreLoadGame").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreLoadGame)).ToLocalChecked());
  Set(target, New("coreUpdate").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreUpdate)).ToLocalChecked());
  Set(target, New("coreVideoData").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreVideoData)).ToLocalChecked());
  Set(target, New("coreVideoSize").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreVideoSize)).ToLocalChecked());
  Set(target, New("coreAudioData").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreAudioData)).ToLocalChecked());
  Set(target, New("coreTimings").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreTimings)).ToLocalChecked());
  Set(target, New("coreSettingsSet").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreSettingsSet)).ToLocalChecked());
  // Set(target, New("coreSettingsDesc").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreSettingsDesc)).ToLocalChecked());
  // Set(target, New("coreJoypadDesc").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreJoypadDesc)).ToLocalChecked());
  // Set(target, New("coreJoypadPress").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreJoypadPress)).ToLocalChecked());
  // Set(target, New("coreJoypadRelease").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreJoypadRelease)).ToLocalChecked());
  // Set(target, New("coreStateSave").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreSaveState)).ToLocalChecked());
  // Set(target, New("coreStateRestore").ToLocalChecked(), GetFunction(New<FunctionTemplate>(nodeCoreRestoreState)).ToLocalChecked());
}

NODE_MODULE(retro_api, init)
