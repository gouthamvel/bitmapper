require 'test/unit'
require 'bitmapper'

class BitmapperTest < Test::Unit::TestCase

  def in_num_file
    "/tmp/ncpr/in.txt"
  end

  def out_num_file
    "/tmp/ncpr/out.txt"
  end

  def in_str_file
    "/tmp/ncpr/in_str.txt"
  end

  def out_str_file
    "/tmp/ncpr/out_str.txt"
  end

  def init_files
    @assert_index_len = 6
    @index_len = 6
    f = open in_num_file, 'w'
    [2,8,9].each do |base|
      (1000).times do |i|
        f.puts base*1_000_000_000 - i
      end
      (1_000).times do |i|
        f.puts base*1_000_000_000 + i
      end
    end
    f.close
  end

  def init_files2
    @assert_index_len = 16
    @index_len = 6
    f = open in_num_file, 'w'
    (2..9).to_a.each do |base|
      f.puts base*1_000_000_000
      f.puts base*1_000_000_000 - 1
      f.puts base*1_000_000_000 + 1
    end
    f.close
  end

  def init_billion_file
    @assert_index_len = 1
    @index_len = 6
    f = open in_num_file, 'w'
    10_000_0.times do |i|
      f.puts 9_000_000_000 + rand(i..100_000_000)
    end
    f.close
  end

  def with_time_count(id='')
    t = Time.now.to_f
    yield
    p "#{id} -- #{Time.now.to_f - t}"
  end

  def test_full_run
    # init_files
    init_billion_file
    map = Bitmapper.new(@index_len)

    with_time_count('add') do
      map.add_from_file in_num_file
    end

    # map.dump_bucket_string out_str_file, map.get_indexes.first
    # map.load_string_to_bucket out_str_file, map.get_indexes.first
    with_time_count('dump') do
      map.dump_to_file out_num_file
    end

    with_time_count('status') do
      p map.status?(9_000_000_000)
      p map.status?('9000000000')
    end
    # assert_equal `bash -c "diff <(sort #{in_num_file}|uniq) <(sort #{out_num_file}|uniq)"`, ''
    # assert_equal map.get_indexes.length, @assert_index_len

    # map.remove_from_file in_num_file
    # map.dump_to_file out_num_file

    # assert_not_equal `bash -c "diff <(sort #{in_num_file}|uniq) <(sort #{out_num_file}|uniq)"`,
    ''
  end

end
