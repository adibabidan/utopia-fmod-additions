#include "fmod_file.h"

#include "helpers/common.h"
#include <deque>

using namespace godot;

void FmodFile::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_length", "lengthtype"), &FmodFile::get_length);
    ClassDB::bind_method(D_METHOD("release"), &FmodFile::release);
    // ClassDB::bind_method(D_METHOD("read_data", "length"), &FmodFile::read_data);
    ClassDB::bind_method(D_METHOD("write_data", "audio_data"), &FmodFile::write_data);
    /*ClassDB::bind_method(D_METHOD("lock", "offset", "length"), &FmodFile::lock);
    ClassDB::bind_method(D_METHOD("unlock", "byte_arr_1", "byte_arr_2"), &FmodFile::unlock);*/
}

unsigned int FmodFile::get_length(FMOD_TIMEUNIT lengthtype) const
{
    unsigned int length{};

    if(ERROR_CHECK(_wrapped->getLength(&length, lengthtype)))
    {
        return length;
    }

    return length;
}

bool FmodFile::release() const
{
    return ERROR_CHECK(_wrapped->release());
}

/*PackedByteArray FmodFile::read_data(unsigned int length) const
{
    void* ptr = new char[length];
    unsigned int read;

    FMOD_RESULT error = _wrapped->readData(ptr, length, &read);
    if(error != FMOD_ERR_FILE_EOF)
    {
        ERROR_CHECK(error);
    }

    PackedByteArray data;
    data.resize(read);

    memcpy(data.begin().operator->(), ptr, data.size());
    return data;
}*/

void FmodFile::write_data(const PackedByteArray& audio_data)
{
    UtilityFunctions::push_warning("pushing data");
    for(unsigned int i = 0; i < audio_data.size(); i++) {
        buffer.push_back(audio_data.ptr()[i]);
    }
}

int FmodFile::pop_from_buffer(void* data, unsigned int datalen)
{
    int to_return = 0;
    int empty_count = 0;
    int buffered_count = 0;
    char* c_data = (char*) data;
    for(unsigned int i = 0; i < datalen; i++)
    {
        if(buffer.empty()) {
            empty_count++;
            to_return = -1;
            c_data[i] = 0;
        }
        else
        {
            buffered_count++;
            c_data[i] = (char) buffer.front();
            buffer.pop_front();
        }
    }
    char debug_string[128];
    sprintf(debug_string, "popping data (empty %i, buffered %i)", empty_count, buffered_count);
    UtilityFunctions::push_warning(debug_string);
    return to_return;
}

/*TypedArray<PackedByteArray> FmodFile::lock(unsigned int offset, unsigned int length) const
{
    TypedArray<PackedByteArray> ret_array = new TypedArray<PackedByteArray>();

    void* ptr1;
    void* ptr2;
    unsigned int len1, len2;

    if(ERROR_CHECK(_wrapped->lock(offset, length, &ptr1, &ptr2, &len1, &len2)))
    {
        PackedByteArray arr1;
        arr1.resize(len1);

        char len1_buffer [33];
        itoa(len1, len1_buffer, 10);
        char len2_buffer [33];
        itoa(len2, len2_buffer, 10);
        UtilityFunctions::push_warning("[LOCK] len1: " + String(len1_buffer) + ", len2: " + String(len2_buffer));

        memcpy(arr1.begin().operator->(), ptr1, arr1.size());
        ret_array.append(arr1);

        if(len2)
        {
            PackedByteArray arr2;
            arr2.resize(len2);

            memcpy(arr2.begin().operator->(), ptr2, arr2.size());
            ret_array.append(arr2);
        }

        return ret_array;
    }

    return ret_array;
}

bool FmodFile::unlock(PackedByteArray byte_arr_1, PackedByteArray byte_arr_2) const
{
    int* arr1_start = (int*) byte_arr_1.begin().operator->();
    int* arr2_start = (int*) byte_arr_2.begin().operator->();

    unsigned int arr1_size = byte_arr_1.size();
    unsigned int arr2_size = byte_arr_2.size();

    if(arr1_size >= sizeof(int) * 4)
    {
        int arr1_a = (*arr1_start) & 255;
        int arr1_b = (*arr1_start >> 4) & 255;
        int arr1_c = (*arr1_start >> 8) & 255;
        int arr1_d = (*arr1_start >> 12) & 255;

        char arr1_char[64];
    }


    char len1_buffer [33];
    itoa(byte_arr_1.size(), len1_buffer, 10);
    char len2_buffer [33];
    itoa(byte_arr_2.size(), len2_buffer, 10);
    UtilityFunctions::push_warning("[UNLOCK] len1: " + String(len1_buffer) + ", len2: " + String(len2_buffer));

    bool res = ERROR_CHECK(_wrapped->unlock(byte_arr_1.begin().operator->(), byte_arr_2.begin().operator->(), arr1_size, arr2_size));

    return res;
}*/