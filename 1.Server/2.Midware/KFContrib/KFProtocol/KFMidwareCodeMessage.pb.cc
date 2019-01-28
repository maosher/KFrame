// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: KFMidwareCodeMessage.proto

#include "KFMidwareCodeMessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace KFMsg {
}  // namespace KFMsg
namespace protobuf_KFMidwareCodeMessage_2eproto {
void InitDefaults() {
}

const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors[1];
const ::google::protobuf::uint32 TableStruct::offsets[1] = {};
static const ::google::protobuf::internal::MigrationSchema* schemas = NULL;
static const ::google::protobuf::Message* const* file_default_instances = NULL;

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "KFMidwareCodeMessage.proto", schemas, file_default_instances, TableStruct::offsets,
      NULL, file_level_enum_descriptors, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\032KFMidwareCodeMessage.proto\022\005KFMsg*\264\005\n\017"
      "MidwareCodeEnum\022\t\n\005Error\020\000\022\006\n\002Ok\020\001\022\022\n\rHt"
      "tpDataError\020\220N\022\025\n\020AuthDatabaseBusy\020\221N\022\023\n"
      "\016AuthServerBusy\020\222N\022\023\n\016ZoneServerBusy\020\223N\022"
      "\025\n\020ZoneDatabaseBusy\020\224N\022\023\n\016AccountIsEmpty"
      "\020\371U\022\026\n\021ActivationAccount\020\372U\022\032\n\025InvalidAc"
      "tivationCode\020\373U\022\030\n\023ActivationCodeError\020\374"
      "U\022\024\n\017LoginTokenError\020\375U\022\023\n\016BanForbidLogi"
      "n\020\376U\022\026\n\021ChannelNotSupport\020\377U\022\023\n\016ChannelN"
      "otOpen\020\200V\022\021\n\014ChannelError\020\201V\022\020\n\013WeiXinEr"
      "ror\020\202V\022\024\n\017WeiXinUserError\020\203V\022\025\n\020WeiXinTo"
      "kenError\020\204V\022\024\n\017WeiXinCodeError\020\205V\022\027\n\022Wei"
      "XinTokenTimeout\020\206V\022\017\n\nSteamError\020\207V\022\023\n\016S"
      "teamDataError\020\210V\022\023\n\016SteamAuthError\020\211V\022\034\n"
      "\027VersionNotCompatibility\020\341]\022\021\n\014LoginIsCl"
      "ose\020\342]\022\027\n\022LoginNoLoginServer\020\343]\022\024\n\017Login"
      "SystemBusy\020\344]\022\031\n\024LoginWorldSystemBusy\020\345]"
      "\022\026\n\021LoginNoGameServer\020\346]\022\030\n\023LoginGameSer"
      "verBusy\020\347]b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 738);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "KFMidwareCodeMessage.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_KFMidwareCodeMessage_2eproto
namespace KFMsg {
const ::google::protobuf::EnumDescriptor* MidwareCodeEnum_descriptor() {
  protobuf_KFMidwareCodeMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_KFMidwareCodeMessage_2eproto::file_level_enum_descriptors[0];
}
bool MidwareCodeEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 10000:
    case 10001:
    case 10002:
    case 10003:
    case 10004:
    case 11001:
    case 11002:
    case 11003:
    case 11004:
    case 11005:
    case 11006:
    case 11007:
    case 11008:
    case 11009:
    case 11010:
    case 11011:
    case 11012:
    case 11013:
    case 11014:
    case 11015:
    case 11016:
    case 11017:
    case 12001:
    case 12002:
    case 12003:
    case 12004:
    case 12005:
    case 12006:
    case 12007:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace KFMsg
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
