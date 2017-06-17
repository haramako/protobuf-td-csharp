# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: test.proto

require 'google/protobuf'

Google::Protobuf::DescriptorPool.generated_pool.build do
  add_message "Test.TestMessage" do
    optional :int32_value, :int32, 1
    optional :int64_value, :int64, 2
    optional :string_value, :string, 3
    optional :enum_value, :enum, 4, "Test.TestEnum"
    optional :message_value, :message, 5, "Test.EmbedMessage"
    optional :sint32_value, :sint32, 6
    optional :sint64_value, :sint64, 7
    optional :bytes_value, :bytes, 8
    repeated :repeated_int32, :int32, 10
    repeated :repeated_int64, :int64, 11
    repeated :repeated_message, :message, 12, "Test.EmbedMessage"
    repeated :packed_int32, :int32, 20
    repeated :packed_int64, :int64, 21
    map :map_value, :int32, :string, 30
  end
  add_message "Test.EmbedMessage" do
    optional :val, :int32, 1
  end
  add_enum "Test.TestEnum" do
    value :FOO, 0
    value :BAR, 1
    value :BAZ, 2
  end
end

module Test
  TestMessage = Google::Protobuf::DescriptorPool.generated_pool.lookup("Test.TestMessage").msgclass
  EmbedMessage = Google::Protobuf::DescriptorPool.generated_pool.lookup("Test.EmbedMessage").msgclass
  TestEnum = Google::Protobuf::DescriptorPool.generated_pool.lookup("Test.TestEnum").enummodule
end
