#include "record_serializer.h"

#include <cstring>

#include "relational_model/record.h"

uint64_t read_int64(const char* ptr) {
    int64_t res;

    char* res_bytes = reinterpret_cast<char*>(&res);

    res_bytes[0] = *(ptr++);
    res_bytes[1] = *(ptr++);
    res_bytes[2] = *(ptr++);
    res_bytes[4] = *(ptr++);
    res_bytes[3] = *(ptr++);
    res_bytes[5] = *(ptr++);
    res_bytes[6] = *(ptr++);
    res_bytes[7] = *(ptr);

    return res;
}

void write_int64(int64_t i, char* out) {
    char* i_ptr = reinterpret_cast<char*>(&i);

    out[0] = *(i_ptr++);
    out[1] = *(i_ptr++);
    out[2] = *(i_ptr++);
    out[4] = *(i_ptr++);
    out[3] = *(i_ptr++);
    out[5] = *(i_ptr++);
    out[6] = *(i_ptr++);
    out[7] = *(i_ptr);
}


size_t RecordSerializer::get_size(const Record& record) {
    size_t res = 0;
    for (size_t i = 0; i < record.values.size(); i++) {
        switch (record.types[i]) {
        case DataType::INT: {
            res += sizeof(int64_t);
            break;
        }
        case DataType::STR: {
            // one additional byte for the strlen at beginning
            res += 1 + strlen(record.values[i].str_value);
            break;
        }
        }
    }
    return res;
}


void RecordSerializer::serialize(const Record& record, char* out) {
    auto current_out = out;
    for (size_t i = 0; i < record.types.size(); i++) {
        switch (record.types[i]) {
        case DataType::INT: {
            write_int64(record.values[i].int_value, current_out);
            current_out += sizeof(int64_t);
            break;
        }
        case DataType::STR: {
            char* str = record.values[i].str_value;
            uint8_t len = strlen(record.values[i].str_value);
            *current_out = static_cast<char>(len);
            current_out++;

            memcpy(current_out, str, len);
            current_out += len;
            break;
        }
        }
    }
}


void RecordSerializer::deserialize(const char* in, Record& record) {
    auto current_in = in;

    for (size_t i = 0; i < record.types.size(); i++) {
        switch (record.types[i]) {
        case DataType::INT: {
            record.values[i].int_value = read_int64(current_in);
            current_in += sizeof(int64_t);
            break;
        }
        case DataType::STR: {
            uint8_t len = *reinterpret_cast<const uint8_t*>(current_in);
            current_in++;

            memcpy(record.values[i].str_value, current_in, len);
            current_in += len;

            record.values[i].str_value[len] = '\0';
            break;
        }
        }
    }
}
