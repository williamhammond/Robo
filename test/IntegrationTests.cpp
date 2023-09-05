#include <gtest/gtest.h>

#include <atomic>
#include <mutex>
#include <thread>

#include "Server.h"
#include "spdlog/spdlog.h"

class IntegrationTests : public ::testing::Test {
 protected:
  void SetUp() override {
    Server::StaticInit();
    serverThread = std::thread([this]() {
      serverReady = true;
      // TODO @WilliamHammond Probably a race condition. We need to add something like an IsReady flag
      // to the engine
      Server::Instance->Run();
    });
    while (!serverReady) {
      std::this_thread::yield();
    }
    SPDLOG_INFO("Integration test fixture ready");
  }

  void TearDown() override {
    Server::Instance->SetQuit(true);
    if (serverThread.joinable()) {
      serverThread.join();
    }
    SPDLOG_INFO("Integration test fixture cleaned up");
  }

  std::thread serverThread;
  std::atomic<bool> serverReady = false;
};

TEST_F(IntegrationTests, BasicAssertions) {
  ASSERT_TRUE(true);
}
