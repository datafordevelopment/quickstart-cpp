// Copyright 2016 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "firebase/analytics.h"
#include "firebase/analytics/event_names.h"
#include "firebase/analytics/parameter_names.h"
#include "firebase/analytics/user_property_names.h"
#include "firebase/app.h"

// Thin OS abstraction layer.
#include "main.h"  // NOLINT

::firebase::App* g_app;

// Execute all methods of the C++ Analytics API.
extern "C" int common_main(int argc, const char* argv[]) {
  namespace analytics = ::firebase::analytics;

  LogMessage("Initialize the Analytics library");
#if defined(__ANDROID__)
  g_app = ::firebase::App::Create(::firebase::AppOptions(FIREBASE_TESTAPP_NAME),
                                  GetJniEnv(), GetActivity());
#else
  g_app =
      ::firebase::App::Create(::firebase::AppOptions(FIREBASE_TESTAPP_NAME));
#endif  // defined(__ANDROID__)

  LogMessage("Created the firebase app %x",
             static_cast<int>(reinterpret_cast<intptr_t>(g_app)));
  analytics::Initialize(*g_app);
  LogMessage("Initialized the firebase analytics API");

  LogMessage("Enabling data collection.");
  analytics::SetAnalyticsCollectionEnabled(true);

  LogMessage("Set user properties.");
  // Set the user's sign up method.
  analytics::SetUserProperty(analytics::kUserPropertySignUpMethod, "Google");
  // Set the user ID.
  analytics::SetUserId("uber_user_510");

  // Log an event with no parameters.
  LogMessage("Log login event.");
  analytics::LogEvent(analytics::kEventLogin);

  // Log an event with a floating point parameter.
  LogMessage("Log progress event.");
  analytics::LogEvent("progress", "percent", 0.4f);

  // Log an event with an integer parameter.
  LogMessage("Log post score event.");
  analytics::LogEvent(analytics::kEventPostScore, analytics::kParameterScore,
                      42);

  // Log an event with a string parameter.
  LogMessage("Log group join event.");
  analytics::LogEvent(analytics::kEventJoinGroup, analytics::kParameterGroupID,
                      "spoon_welders");

  // Log an event with multiple parameters.
  LogMessage("Log level up event.");
  {
    const analytics::Parameter kLevelUpParameters[] = {
        analytics::Parameter(analytics::kParameterLevel, 5),
        analytics::Parameter(analytics::kParameterCharacter, "mrspoon"),
        analytics::Parameter("hit_accuracy", 3.14f),
    };
    analytics::LogEvent(
        analytics::kEventLevelUp, kLevelUpParameters,
        sizeof(kLevelUpParameters) / sizeof(kLevelUpParameters[0]));
  }
  return 0;
}
