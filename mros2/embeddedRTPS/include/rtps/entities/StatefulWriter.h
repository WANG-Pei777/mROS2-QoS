/*
The MIT License
Copyright (c) 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE

This file is part of embeddedRTPS.

Author: i11 - Embedded Software, RWTH Aachen University
*/

#ifndef RTPS_STATEFULWRITER_H
#define RTPS_STATEFULWRITER_H

#include "rtps/entities/ReaderProxy.h"
#include "rtps/entities/Writer.h"
#include "rtps/storages/MemoryPool.h"
#include "rtps/storages/SimpleHistoryCache.h"

namespace rtps {

template <class NetworkDriver> class StatefulWriterT final : public Writer {
public:
  ~StatefulWriterT() override;
  bool init(TopicData attributes, TopicKind_t topicKind, ThreadPool *threadPool,
            NetworkDriver &driver, bool enfUnicast = false);

  // QoS configuration
  void setDeadlineMs(uint32_t ms) { m_deadlineMs = ms; }
  void setLifespanMs(uint32_t ms) { m_lifespanMs = ms; }
  void setLivelinessLeaseMs(uint32_t ms) { m_livelinessLeaseMs = ms; }
  void setResourceLimits(uint32_t maxSamples, uint32_t maxBytes) {
    m_maxSamples = maxSamples;
    m_maxBytes = maxBytes;
  }
  void setKeepAll(bool keepAll) { m_keepAll = keepAll; }
  void setHistoryDepth(uint32_t depth) override { m_historyDepth = depth; }

  // QoS stats
  uint32_t getDeadlineMissedCount() const override { return m_deadlineMissedCount; }
  uint32_t getLifespanDropCount() const override { return m_lifespanDropCount; }
  uint32_t getResourceRejectCount() const override { return m_resourceRejectCount; }
  uint32_t getHistoryDepth() const override { return m_historyDepth; }
  uint32_t getHistoryCount() const override { return m_history.getChangeCount(); }
  uint32_t getHistoryBytes() const override { return m_history.getTotalDataBytes(); }
  bool isAlive() const { return m_isAlive; }
  // Liveliness: manually assert that this writer is alive
  void assertLiveliness() { m_isAlive = true; m_lastPublishTimeMs = rtps::getCurrentTimeMs(); }

  bool addNewMatchedReader(const ReaderProxy &newProxy) override;
  void removeReader(const Guid_t &guid) override;
  void removeReaderOfParticipant(const GuidPrefix_t &guidPrefix) override;
  void progress() override;
  const CacheChange *newChange(ChangeKind_t kind, const uint8_t *data,
                               DataSize_t size) override;
  const CacheChange *newChangeCallback(ChangeKind_t kind,
				       CacheChange::SerializerCallback func, FragDataSize_t size) override;
  void setAllChangesToUnsent() override;
  void onNewAckNack(const SubmessageAckNack &msg,
                    const GuidPrefix_t &sourceGuidPrefix) override;

private:
  sys_mutex_t m_mutex;
  ThreadPool *mp_threadPool = nullptr;

  PacketInfo m_packetInfo;
  NetworkDriver *m_transport;
  bool m_enforceUnicast;

  TopicKind_t m_topicKind = TopicKind_t::NO_KEY;
  SequenceNumber_t m_nextSequenceNumberToSend = {0, 1};
  SimpleHistoryCache<Config::HISTORY_SIZE_STATEFUL> m_history;
  sys_thread_t m_heartbeatThread;
  Count_t m_hbCount{1};

  bool m_running = true;
  bool m_thread_running = false;

  // QoS state
  bool m_keepAll = false;
  uint32_t m_historyDepth = 0;
  uint32_t m_deadlineMs = 0;
  uint64_t m_lastPublishTimeMs = 0;
  uint64_t m_nextDeadlineTimeMs = 0;
  uint32_t m_deadlineMissedCount = 0;
  uint32_t m_lifespanMs = 0;
  uint32_t m_lifespanDropCount = 0;
  uint32_t m_livelinessLeaseMs = 0;
  bool m_isAlive = true;
  uint32_t m_maxSamples = 0;
  uint32_t m_maxBytes = 0;
  uint32_t m_resourceRejectCount = 0;

  bool sendData(const ReaderProxy &reader, const SequenceNumber_t &sn);
  bool sendDataWRMulticast(const ReaderProxy &reader,
                           const SequenceNumber_t &sn);
  static void hbFunctionJumppad(void *thisPointer);
  void sendHeartBeatLoop();
  void sendHeartBeat();
  bool isIrrelevant(ChangeKind_t kind) const;
  void manageSendOptions();
  void resetSendOptions();
};

using StatefulWriter = StatefulWriterT<UdpDriver>;
} // namespace rtps

#include "StatefulWriter.tpp"

#endif // RTPS_STATEFULWRITER_H
