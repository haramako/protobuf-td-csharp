#!/usr/bin/env ruby

# make test data

require 'google/protobuf'
require_relative 'test'

test_data = Test::TestMessage.new(
  int32_value: 1,
  int64_value: 2,
  string_value: 'hoge',
  enum_value: :BAR,
  message_value: Test::EmbedMessage.new(val: 999999)
)

print Test::TestMessage.encode(test_data)
