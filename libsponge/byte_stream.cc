#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity): _buf(), _limit(capacity), _remain(capacity), _written(0), _popped(0), _done(false), _error(false) {}

size_t ByteStream::write(const string &data) {
    if (data.size() <= _remain) {
        _buf.push_back(data);
        _remain -= data.size();
        _written += data.size();
        return data.size();
    } else {
        int remain = _remain;
        _buf.push_back(data.substr(0, remain));
        _written += remain;
        _remain = 0;
        return remain;
    }    
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string ans;
    size_t tmp = len;
    for (auto str : _buf) {
        if (str.size() < tmp) {
            ans += str;
            tmp -= str.size();
        } else if (str.size() == tmp) {
            return ans + str;
        } else {
            return ans + str.substr(0, tmp);
        }
    }
    return ans;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    size_t tmp = len;
    while (_buf.size()) {
        string &front = _buf.front();
        if (front.size() < tmp) {
            tmp -= front.size();
            _popped += front.size();
            _remain += front.size();
            _buf.pop_front();
        } else if (front.size() == tmp) {
            _popped += front.size();
            _remain += front.size();
            _buf.pop_front();
            return;
        } else {
            _popped += tmp;
            _remain += tmp;
            front = front.substr(tmp);
            return;
        }
    }
    return;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string buf;
    for (size_t i = 0; i < len; ++i) {
        char ch;
        cin >> ch;
        buf += ch;
    }
    return buf;
}

void ByteStream::end_input() { _done = true; }

bool ByteStream::input_ended() const { return _done; }

size_t ByteStream::buffer_size() const { return _limit - _remain; }

bool ByteStream::buffer_empty() const { return _remain == _limit; }

bool ByteStream::eof() const { return _done && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _written; }

size_t ByteStream::bytes_read() const { return _popped; }

size_t ByteStream::remaining_capacity() const { return _remain; }
