// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: KFEnumMessage.proto

#ifndef PROTOBUF_INCLUDED_KFEnumMessage_2eproto
#define PROTOBUF_INCLUDED_KFEnumMessage_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_KFEnumMessage_2eproto LIBPROTOC_EXPORT

namespace protobuf_KFEnumMessage_2eproto {
// Internal implementation detail -- do not use these members.
struct LIBPROTOC_EXPORT TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void LIBPROTOC_EXPORT AddDescriptors();
}  // namespace protobuf_KFEnumMessage_2eproto
namespace KFMsg {
}  // namespace KFMsg
namespace KFMsg {

enum ChannelEnum {
  UnknowChannel = 0,
  Internal = 1,
  WeiXin = 2,
  Steam = 3,
  ChannelEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  ChannelEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool ChannelEnum_IsValid(int value);
const ChannelEnum ChannelEnum_MIN = UnknowChannel;
const ChannelEnum ChannelEnum_MAX = Steam;
const int ChannelEnum_ARRAYSIZE = ChannelEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* ChannelEnum_descriptor();
inline const ::std::string& ChannelEnum_Name(ChannelEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    ChannelEnum_descriptor(), value);
}
inline bool ChannelEnum_Parse(
    const ::std::string& name, ChannelEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ChannelEnum>(
    ChannelEnum_descriptor(), name, value);
}
enum SexEnum {
  UnknowSex = 0,
  Male = 1,
  Female = 2,
  SexEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SexEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool SexEnum_IsValid(int value);
const SexEnum SexEnum_MIN = UnknowSex;
const SexEnum SexEnum_MAX = Female;
const int SexEnum_ARRAYSIZE = SexEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* SexEnum_descriptor();
inline const ::std::string& SexEnum_Name(SexEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    SexEnum_descriptor(), value);
}
inline bool SexEnum_Parse(
    const ::std::string& name, SexEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SexEnum>(
    SexEnum_descriptor(), name, value);
}
enum KickEnum {
  UnknowKick = 0,
  LoginBeKick = 1,
  PlatformBeKick = 2,
  KickEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  KickEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool KickEnum_IsValid(int value);
const KickEnum KickEnum_MIN = UnknowKick;
const KickEnum KickEnum_MAX = PlatformBeKick;
const int KickEnum_ARRAYSIZE = KickEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* KickEnum_descriptor();
inline const ::std::string& KickEnum_Name(KickEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    KickEnum_descriptor(), value);
}
inline bool KickEnum_Parse(
    const ::std::string& name, KickEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<KickEnum>(
    KickEnum_descriptor(), name, value);
}
enum FlagEnum {
  Init = 0,
  Done = 1,
  Received = 2,
  Remove = 3,
  ReceiveRemove = 4,
  FlagEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  FlagEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool FlagEnum_IsValid(int value);
const FlagEnum FlagEnum_MIN = Init;
const FlagEnum FlagEnum_MAX = ReceiveRemove;
const int FlagEnum_ARRAYSIZE = FlagEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* FlagEnum_descriptor();
inline const ::std::string& FlagEnum_Name(FlagEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    FlagEnum_descriptor(), value);
}
inline bool FlagEnum_Parse(
    const ::std::string& name, FlagEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<FlagEnum>(
    FlagEnum_descriptor(), name, value);
}
enum MailEnum {
  UnknowMail = 0,
  WholeMail = 1,
  FriendMail = 2,
  GiftMail = 3,
  MailEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  MailEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool MailEnum_IsValid(int value);
const MailEnum MailEnum_MIN = UnknowMail;
const MailEnum MailEnum_MAX = GiftMail;
const int MailEnum_ARRAYSIZE = MailEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* MailEnum_descriptor();
inline const ::std::string& MailEnum_Name(MailEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    MailEnum_descriptor(), value);
}
inline bool MailEnum_Parse(
    const ::std::string& name, MailEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MailEnum>(
    MailEnum_descriptor(), name, value);
}
enum StatusEnum {
  UnknowStatus = 0,
  OnlineStatus = 1,
  GroupStatus = 2,
  MatchStatus = 3,
  PlayingStatus = 4,
  OfflineStatus = 5,
  StatusEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  StatusEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool StatusEnum_IsValid(int value);
const StatusEnum StatusEnum_MIN = UnknowStatus;
const StatusEnum StatusEnum_MAX = OfflineStatus;
const int StatusEnum_ARRAYSIZE = StatusEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* StatusEnum_descriptor();
inline const ::std::string& StatusEnum_Name(StatusEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    StatusEnum_descriptor(), value);
}
inline bool StatusEnum_Parse(
    const ::std::string& name, StatusEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<StatusEnum>(
    StatusEnum_descriptor(), name, value);
}
enum InviteEnum {
  UnknowInvite = 0,
  Consent = 1,
  Refuse = 2,
  Delete = 3,
  RefuseMinute = 4,
  InviteEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  InviteEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool InviteEnum_IsValid(int value);
const InviteEnum InviteEnum_MIN = UnknowInvite;
const InviteEnum InviteEnum_MAX = RefuseMinute;
const int InviteEnum_ARRAYSIZE = InviteEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* InviteEnum_descriptor();
inline const ::std::string& InviteEnum_Name(InviteEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    InviteEnum_descriptor(), value);
}
inline bool InviteEnum_Parse(
    const ::std::string& name, InviteEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<InviteEnum>(
    InviteEnum_descriptor(), name, value);
}
enum StoreWishEnum {
  UnknowStore = 0,
  Add = 1,
  Del = 2,
  Modify = 3,
  StoreWishEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  StoreWishEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool StoreWishEnum_IsValid(int value);
const StoreWishEnum StoreWishEnum_MIN = UnknowStore;
const StoreWishEnum StoreWishEnum_MAX = Modify;
const int StoreWishEnum_ARRAYSIZE = StoreWishEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* StoreWishEnum_descriptor();
inline const ::std::string& StoreWishEnum_Name(StoreWishEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    StoreWishEnum_descriptor(), value);
}
inline bool StoreWishEnum_Parse(
    const ::std::string& name, StoreWishEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<StoreWishEnum>(
    StoreWishEnum_descriptor(), name, value);
}
enum WishStateEnum {
  AllWatch = 0,
  NoneWatch = 1,
  FriendWatch = 2,
  TeamWatch = 3,
  WishStateEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  WishStateEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool WishStateEnum_IsValid(int value);
const WishStateEnum WishStateEnum_MIN = AllWatch;
const WishStateEnum WishStateEnum_MAX = TeamWatch;
const int WishStateEnum_ARRAYSIZE = WishStateEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* WishStateEnum_descriptor();
inline const ::std::string& WishStateEnum_Name(WishStateEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    WishStateEnum_descriptor(), value);
}
inline bool WishStateEnum_Parse(
    const ::std::string& name, WishStateEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<WishStateEnum>(
    WishStateEnum_descriptor(), name, value);
}
enum FriendLinessEnum {
  UnKnowFriend = 0,
  Team = 1,
  Win = 2,
  Toast = 3,
  Give = 4,
  FriendLinessEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  FriendLinessEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool FriendLinessEnum_IsValid(int value);
const FriendLinessEnum FriendLinessEnum_MIN = UnKnowFriend;
const FriendLinessEnum FriendLinessEnum_MAX = Give;
const int FriendLinessEnum_ARRAYSIZE = FriendLinessEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* FriendLinessEnum_descriptor();
inline const ::std::string& FriendLinessEnum_Name(FriendLinessEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    FriendLinessEnum_descriptor(), value);
}
inline bool FriendLinessEnum_Parse(
    const ::std::string& name, FriendLinessEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<FriendLinessEnum>(
    FriendLinessEnum_descriptor(), name, value);
}
enum GuildEnum {
  UnKnowGuild = 0,
  JoinGuild = 1,
  ExitGuild = 2,
  GuildEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  GuildEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool GuildEnum_IsValid(int value);
const GuildEnum GuildEnum_MIN = UnKnowGuild;
const GuildEnum GuildEnum_MAX = ExitGuild;
const int GuildEnum_ARRAYSIZE = GuildEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* GuildEnum_descriptor();
inline const ::std::string& GuildEnum_Name(GuildEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    GuildEnum_descriptor(), value);
}
inline bool GuildEnum_Parse(
    const ::std::string& name, GuildEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GuildEnum>(
    GuildEnum_descriptor(), name, value);
}
enum GuildReviewEnum {
  UnknowGuildReview = 0,
  AgreeApply = 1,
  RefuseApply = 2,
  GuildReviewEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  GuildReviewEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool GuildReviewEnum_IsValid(int value);
const GuildReviewEnum GuildReviewEnum_MIN = UnknowGuildReview;
const GuildReviewEnum GuildReviewEnum_MAX = RefuseApply;
const int GuildReviewEnum_ARRAYSIZE = GuildReviewEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* GuildReviewEnum_descriptor();
inline const ::std::string& GuildReviewEnum_Name(GuildReviewEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    GuildReviewEnum_descriptor(), value);
}
inline bool GuildReviewEnum_Parse(
    const ::std::string& name, GuildReviewEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GuildReviewEnum>(
    GuildReviewEnum_descriptor(), name, value);
}
enum BattleEnum {
  BattleUnknow = 0,
  OpenSuccess = 1,
  OpenFailed = 2,
  BattleEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  BattleEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool BattleEnum_IsValid(int value);
const BattleEnum BattleEnum_MIN = BattleUnknow;
const BattleEnum BattleEnum_MAX = OpenFailed;
const int BattleEnum_ARRAYSIZE = BattleEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* BattleEnum_descriptor();
inline const ::std::string& BattleEnum_Name(BattleEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    BattleEnum_descriptor(), value);
}
inline bool BattleEnum_Parse(
    const ::std::string& name, BattleEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<BattleEnum>(
    BattleEnum_descriptor(), name, value);
}
enum CodeEnum {
  Error = 0,
  Ok = 1,
  SignError = 2,
  WeiXinError = 3,
  WeiXinCodeError = 4,
  WeiXinTokenTimeout = 5,
  WeiXinTokenError = 6,
  WeiXinUserError = 7,
  ChannelError = 8,
  ChannelNotOpen = 9,
  WorldSystemBusy = 10,
  LoginSystemBusy = 11,
  CanNotFindProxy = 12,
  AuthServerBusy = 13,
  AuthDatabaseBusy = 14,
  CanNotFindGame = 15,
  LoginTokenError = 16,
  LoadDataFailed = 17,
  HttpDataError = 18,
  LoginDatabaseError = 19,
  CreatePlayerError = 20,
  SexSetOK = 21,
  NameAlreadySet = 22,
  NameAlreadyExist = 23,
  PublicDatabaseError = 24,
  NameSetOK = 25,
  AccountIsEmpty = 26,
  CreateRoleAlready = 27,
  CreateRoleOK = 28,
  LoginAlreadyOnline = 29,
  ChangeIconOK = 30,
  NameEmpty = 31,
  ChangeMottoOK = 32,
  ModuleIdNotFind = 33,
  SetModelClothesOK = 34,
  ChangeIconBoxOK = 35,
  DataSystemBusy = 36,
  ActivationAccount = 37,
  ActivationCodeError = 38,
  LoginCanNotFindGate = 39,
  TaskIdCanNotFind = 40,
  TaskNotInValidTime = 41,
  CanNotFindPlayer = 42,
  CanNotFindTaskData = 43,
  TaskNotDone = 44,
  TaskAlreadyReceived = 45,
  TaskReceiveRewardOK = 46,
  CanNotInquireSelf = 47,
  ActivityIdCanNotFind = 48,
  CanNotFindActivityType = 49,
  ActivityAlreadyReceived = 50,
  ActivityNotDone = 51,
  ActivityReceiveRewardOK = 52,
  AchieveIdCanNotFind = 53,
  CanNotFindAchieveData = 54,
  AchieveAlreadyReceived = 55,
  AchieveNotDone = 56,
  AchieveReceiveRewardOK = 57,
  ReceiveGiftOK = 58,
  ReceiveGiftFailed = 59,
  MailNotExist = 60,
  MailAlreadyReceived = 61,
  MailNotHaveReward = 62,
  MailDeleteFailed = 63,
  MailTimeOut = 64,
  ToastSendCountOver = 65,
  ToastCanNotRepeat = 66,
  ToastGetCountOver = 67,
  ToastFailed = 68,
  ToastOK = 69,
  MatchAlreadyWait = 70,
  MatchClusterBusy = 71,
  MatchCanNotFindServer = 72,
  MatchCanNotFindMatch = 73,
  MatchGroupPlayerLimit = 74,
  MatchRequestSuccess = 75,
  MatchCancelSuccess = 76,
  FriendInviteOK = 77,
  FriendRefuseInvite = 78,
  FriendTargetLimit = 79,
  FriendSelfLimit = 80,
  FriendAlready = 81,
  FriendNotExist = 82,
  FriendDelOK = 83,
  FriendInviteLimit = 84,
  FriendInviteNotExist = 85,
  MatchNotInMatch = 86,
  FriendInviteAlready = 87,
  MatchMustPrepare = 88,
  StoreParamError = 89,
  StoreLackCost = 90,
  StoreOutOfLimits = 91,
  StoreOutOfLimitOwm = 92,
  StoreOutOfLimitDate = 93,
  WishOrderEmpty = 94,
  WishOrderNoExist = 95,
  WishOrderNoStoreId = 96,
  WishOrderHadStoreId = 97,
  WishOrderMaxSize = 98,
  WishOrderStateRepeatSet = 99,
  GiveLackCost = 100,
  ClothesNotExist = 101,
  ClothesDataError = 102,
  ClothesModleNotMatch = 103,
  ClothesDressOK = 104,
  ModelNotExist = 105,
  ModelChangeOK = 106,
  ChatByteTooLong = 107,
  ChatNotFindPlayer = 108,
  ChatIntervalError = 109,
  ChatFriendIdError = 110,
  InviteMatchGroupOK = 111,
  GroupLeaveOK = 112,
  GroupMemberIsFull = 113,
  GroupServerBusy = 114,
  GroupInviteNotExist = 115,
  GroupCanNotKickSelf = 116,
  GroupInGroup = 117,
  GroupApplyNotExist = 118,
  GroupNotInGroup = 119,
  ItemNotExist = 120,
  ItemDataError = 121,
  ItemCanNotUse = 122,
  GroupNotExist = 123,
  StoreBuyOK = 124,
  GiveBuyOK = 125,
  GroupPlayerOffline = 126,
  GroupAlreadyInGroup = 127,
  GroupRefuseInvite = 128,
  GroupRefuseYourInvite = 129,
  GroupMemberLeave = 130,
  GroupRefuseApply = 131,
  GroupCaptainOffline = 132,
  GroupNoCaption = 133,
  GroupNotCaption = 134,
  FriendServerBusy = 135,
  FriendRefuseYourInvite = 136,
  FriendAddOK = 137,
  FriendInviteReq = 138,
  GroupJoinOK = 139,
  GroupMemberJoin = 140,
  GroupIsNewCaption = 141,
  FriendLinessAdd = 142,
  FriendLinessAddLimit = 143,
  FriendLinessAddByWinGame = 144,
  FriendLinessMaxLimit = 145,
  WishPanelAddSuccessed = 146,
  GroupInviteInMatch = 147,
  GroupApplyInMatch = 148,
  GroupInviteTimeOut = 149,
  GroupInviteMatchError = 150,
  GroupApplyTimeOut = 151,
  OperateFrequently = 152,
  DataNotEnough = 153,
  CompoundNotExist = 154,
  LoginIsClose = 155,
  RankNotExist = 156,
  RankServerBusy = 157,
  SignInNotDay = 161,
  SignInDataError = 162,
  SignInRewardAlready = 163,
  DirServerBusy = 164,
  GroupTargetInMatch = 165,
  GroupInMatch = 166,
  GroupKickInMatch = 167,
  GroupLeaveInMatch = 168,
  MatchCancelInBattle = 169,
  MailServerBusy = 170,
  RelationDatabaseBusy = 171,
  InvalidFilter = 172,
  SteamError = 173,
  SteamDataError = 174,
  SteamAuthError = 175,
  InvalidActivationCode = 176,
  CanNotFindLoginNode = 177,
  ChannelNotSupport = 178,
  HaveNotIcon = 179,
  QueryInviteScoreError = 180,
  PlatformServerBusy = 181,
  BindAlipayOk = 182,
  BindAlipayFailed = 183,
  ExchangeInviteScoreError = 184,
  ExchangeInviteRecordError = 185,
  VersionNotCompatibility = 186,
  BattleNotCompatibility = 187,
  BattleNotPrepare = 188,
  BanForbidLogin = 189,
  TitleNotExist = 190,
  ServerBusy = 1000,
  CodeEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  CodeEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
LIBPROTOC_EXPORT bool CodeEnum_IsValid(int value);
const CodeEnum CodeEnum_MIN = Error;
const CodeEnum CodeEnum_MAX = ServerBusy;
const int CodeEnum_ARRAYSIZE = CodeEnum_MAX + 1;

LIBPROTOC_EXPORT const ::google::protobuf::EnumDescriptor* CodeEnum_descriptor();
inline const ::std::string& CodeEnum_Name(CodeEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    CodeEnum_descriptor(), value);
}
inline bool CodeEnum_Parse(
    const ::std::string& name, CodeEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CodeEnum>(
    CodeEnum_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace KFMsg

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::KFMsg::ChannelEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::ChannelEnum>() {
  return ::KFMsg::ChannelEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::SexEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::SexEnum>() {
  return ::KFMsg::SexEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::KickEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::KickEnum>() {
  return ::KFMsg::KickEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::FlagEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::FlagEnum>() {
  return ::KFMsg::FlagEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::MailEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::MailEnum>() {
  return ::KFMsg::MailEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::StatusEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::StatusEnum>() {
  return ::KFMsg::StatusEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::InviteEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::InviteEnum>() {
  return ::KFMsg::InviteEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::StoreWishEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::StoreWishEnum>() {
  return ::KFMsg::StoreWishEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::WishStateEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::WishStateEnum>() {
  return ::KFMsg::WishStateEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::FriendLinessEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::FriendLinessEnum>() {
  return ::KFMsg::FriendLinessEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::GuildEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::GuildEnum>() {
  return ::KFMsg::GuildEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::GuildReviewEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::GuildReviewEnum>() {
  return ::KFMsg::GuildReviewEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::BattleEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::BattleEnum>() {
  return ::KFMsg::BattleEnum_descriptor();
}
template <> struct is_proto_enum< ::KFMsg::CodeEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::KFMsg::CodeEnum>() {
  return ::KFMsg::CodeEnum_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_KFEnumMessage_2eproto