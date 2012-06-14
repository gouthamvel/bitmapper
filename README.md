# Bitmapper
    A Fast, In-memory, Bitmap tool.(For: Number(Integers) filtering).

## Usage
currently supports max 10 digits numbers
### Creating the bitmap

    map = Bitmapper.new(3)

3 -> is the number of digits from left(most significant digits) which will be considered as index

### operation on files

In a nutshell giving a index ~= number of digits/2 will show good time and memory performance. See working to know more.

    map.add("/tmp/file_with_numbers")

adds the numbers in the given file to bitmap.
Similar operations

    map.remove("/tmp/file_with_numbers")
    map.dump_to("/tmp/file_name")

### operation on binary strings
Apart from numbers the bitmap's binary sequence can be dumped to file.

    map.dump_to_str("/tmp/file_name", <bucket_index>)
    map.load_from_str("/tmp/file_with_binary_string, <bucket_index>)

the bucket_index is the ID of the bucket to dump.
dumping the whole bitmap to binary string is still pending.

### operation on each number

    map.set 9876543210
    map.set "987654321"0
    map.clear 98765432100
    map.status? 987654321 # returns true/false

raises TypeError, for number more than 10 digits

## Working
Number is split in two based on the index length given at the begining.

    Ex: 9876543210 -> 987:6543210
    987 -> this is the index of bucket in which rest of number is stored.
    6543210 -> this is the offset of the bit in 987 bucket.

987 bucket is created when the first time a 987xxxxxxx number is given.

## TODO
    * Check file_exist? for input files
    * eliminate the parameter in new
    * consider padding for numbers with lesser digits
