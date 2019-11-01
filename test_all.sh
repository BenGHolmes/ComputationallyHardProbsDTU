g++ sswe.cpp -o sswe -std=c++11

for filename in tests/*.swe; do
    echo "$filename"
    ./sswe < "$filename"
    echo "\n\n"
done