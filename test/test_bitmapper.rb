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

  def init_million_file
    @assert_index_len = 1
    @index_len = 6
    f = open in_num_file, 'w'
    10_000_00.times do |i|
      f.puts 9_000_000_000 + rand(i..100_000_000)
    end
    f.close
  end

  def with_time_count(id='')
    t = Time.now.to_f
    yield
    p "#{id} -- #{Time.now.to_f - t}"
  end
  def setup
    (init_files2 and @create_file = true) unless @create_file
    @invalid_max_number = 9_000_000_000_0
    @invalid_char_number = '9_x00_000_000_0'
    @map = Bitmapper.new(@index_len)
  end

  def test_get_indexes
    @map.add_from_file in_num_file
    assert_equal @map.get_indexes.length, @assert_index_len
  end

  def test_add_from_file
    @map.add_from_file in_num_file
    numbers = `cat #{in_num_file}`.split("\n")
    numbers.each do |i|
      assert_equal @map.status?(i), true
    end
  end

  def test_remove_from_file
    @map.add_from_file in_num_file
    @map.remove_from_file in_num_file
    numbers = `cat #{in_num_file}`.split("\n")
    numbers.each do |i|
      assert_equal @map.status?(i), false
    end
  end

  def test_dump_to_file
    @map.add_from_file in_num_file
    @map.dump_to_file out_num_file
    assert_equal `bash -c "diff <(sort #{in_num_file}|uniq) <(sort #{out_num_file}|uniq)"`, ''
  end

  def test_dump_bucket_str

  end

  def test_dump_load_str_to_bucket
  end

  def test_status
    @map.add_from_file in_num_file
    first_10_num = `head -n10 #{in_num_file}`.split("\n")[0..10]
    first_10_num.each do |i|
      assert_equal @map.status?(i), true
    end
    assert_equal @map.status?(@invalid_max_number), false
    assert_equal @map.status?(@invalid_char_number), false
  end

  def test_set_num
    [9_990_000_111, '9_990_000_111'].each do |i|
      @map.set i
      assert_equal @map.status?(i), true
    end
    assert_equal @map.set(@invalid_max_number), false
    assert_equal @map.set(@invalid_char_number), false
  end

  def test_clear_num
    [9_990_000_111, '9_990_000_111'].each{ |i| @map.set i}
    [9_990_000_111, '9_990_000_111'].each do |i|
      @map.clear i
      assert_equal @map.status?(i), false
    end
    assert_equal @map.clear(@invalid_max_number), false
    assert_equal @map.clear(@invalid_char_number), false
  end

  def test_full_run
    with_time_count('add') do
      @map.add_from_file in_num_file
    end
    @map.dump_bucket_string out_str_file, @map.get_indexes.first
    @map.load_string_to_bucket out_str_file, @map.get_indexes.first
    with_time_count('dump') do
      @map.dump_to_file out_num_file
    end
    first_10_num = `head -n10 #{in_num_file}`.split("\n")[0..10]
    first_10_num.each do |i|
      assert_equal @map.status?(i), true
    end
    assert_equal `bash -c "diff <(sort #{in_num_file}|uniq) <(sort #{out_num_file}|uniq)"`, ''
    assert_equal @map.get_indexes.length, @assert_index_len

    @map.remove_from_file in_num_file
    @map.dump_to_file out_num_file

    assert_not_equal `bash -c "diff <(sort #{in_num_file}|uniq) <(sort #{out_num_file}|uniq)"`,''

  end

end
