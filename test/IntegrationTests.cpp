#include <gtest/gtest.h>

#include <mutex>
#include <thread>

#include "Server.h"

class IntegrationTests : public ::testing::Test {
 protected:
  void SetUp() override {
    Server::StaticInit();
    serverThread = std::thread([this]() {
      serverInitializationCV.notify_one();
      serverReady = true;
      // TODO @WilliamHammond Probably a race condition. We need to add something like an IsReady flag
      // to the engine
      Server::Instance->Run();
    });
    std::unique_lock<std::mutex> lock(mutex);
    serverInitializationCV.wait(lock, [this]() { return serverReady; });
  }

  void TearDown() override {
    Server::Instance->SetQuit(true);
    if (serverThread.joinable()) {
      serverThread.join();
    }
  }

  std::condition_variable serverInitializationCV;
  std::mutex mutex;
  std::thread serverThread;
  bool serverReady = false;
};

TEST_F(IntegrationTests, BasicAssertions) {
  ASSERT_TRUE(true);
}
