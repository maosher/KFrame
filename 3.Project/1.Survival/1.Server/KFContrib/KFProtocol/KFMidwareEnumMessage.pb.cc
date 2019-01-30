// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: KFMidwareEnumMessage.proto

#include "KFMidwareEnumMessage.pb.h"

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
namespace protobuf_KFMidwareEnumMessage_2eproto {
void InitDefaults() {
}

const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors[5];
const ::google::protobuf::uint32 TableStruct::offsets[1] = {};
static const ::google::protobuf::internal::MigrationSchema* schemas = NULL;
static const ::google::protobuf::Message* const* file_default_instances = NULL;

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "KFMidwareEnumMessage.proto", schemas, file_default_instances, TableStruct::offsets,
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
      "\n\032KFMidwareEnumMessage.proto\022\005KFMsg*E\n\013C"
      "hannelEnum\022\021\n\rUnknowChannel\020\000\022\014\n\010Interna"
      "l\020\001\022\n\n\006WeiXin\020\002\022\t\n\005Steam\020\003*\?\n\010KickEnum\022\016"
      "\n\nUnknowKick\020\000\022\017\n\013KickByLogin\020\001\022\022\n\016KickB"
      "yPlatform\020\002*^\n\nStatusEnum\022\016\n\nInitStatus\020"
      "\000\022\016\n\nDoneStatus\020\001\022\021\n\rReceiveStatus\020\002\022\n\n\006"
      "Remove\020\003\022\021\n\rReceiveRemove\020\004*v\n\010UUIDEnum\022"
      "\r\n\tUUidLogic\020\000\022\017\n\013UUidAccount\020\001\022\016\n\nUUidP"
      "layer\020\002\022\014\n\010UUidItem\020\003\022\r\n\tUUidGroup\020\004\022\r\n\t"
      "UUidGuild\020\005\022\016\n\nUUidBattle\020\006*:\n\010MailEnum\022"
      "\016\n\nUnknowMail\020\000\022\016\n\nGlobalMail\020\001\022\016\n\nPerso"
      "nMail\020\002b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 455);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "KFMidwareEnumMessage.proto", &protobuf_RegisterTypes);
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
}  // namespace protobuf_KFMidwareEnumMessage_2eproto
namespace KFMsg {
const ::google::protobuf::EnumDescriptor* ChannelEnum_descriptor() {
  protobuf_KFMidwareEnumMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_KFMidwareEnumMessage_2eproto::file_level_enum_descriptors[0];
}
bool ChannelEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* KickEnum_descriptor() {
  protobuf_KFMidwareEnumMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_KFMidwareEnumMessage_2eproto::file_level_enum_descriptors[1];
}
bool KickEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* StatusEnum_descriptor() {
  protobuf_KFMidwareEnumMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_KFMidwareEnumMessage_2eproto::file_level_enum_descriptors[2];
}
bool StatusEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* UUIDEnum_descriptor() {
  protobuf_KFMidwareEnumMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_KFMidwareEnumMessage_2eproto::file_level_enum_descriptors[3];
}
bool UUIDEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* MailEnum_descriptor() {
  protobuf_KFMidwareEnumMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_KFMidwareEnumMessage_2eproto::file_level_enum_descriptors[4];
}
bool MailEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
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
