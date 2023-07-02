#include "absl/status/status.h"

#include <iostream>

#include "absl/strings/cord.h"
#include "absl/strings/numbers.h"

absl::Status func() {
  return absl::Status{absl::StatusCode::kNotFound, "hello"};
}

enum class Tag : uint8_t {
  // Sending end of the socket was closed, so receiving end can no longer read.
  kSocketEof = 0,

  // Bind failed, probably because there's another instance of cdc_rsync
  // running.
  kAddressInUse = 1,

  // The gamelet components need to be re-deployed.
  kDeployServer = 2,

  // Timeout while trying to connect to the gamelet component.
  kConnectionTimeout = 3,

  // MUST BE LAST.
  kCount = 4,
};

constexpr char kTagKey[] = "tag";

absl::Cord TagToCord(Tag tag) {
  return absl::Cord(std::to_string(static_cast<int>(tag)));
}

absl::Status SetTag(absl::Status status, Tag tag) {
  status.SetPayload(kTagKey, TagToCord(tag));
  return status;
}

//------------------------------

// --------------

int main() {
  return 0;
  auto st = func();
  std::cout << (int)st.code() << ": " << st.message() << std::endl;
  SetTag(st, Tag::kAddressInUse);
  st.GetPayload(kTagKey);
  return 0;
}