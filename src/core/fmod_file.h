#ifndef GODOTFMOD_FMOD_FILE_H
#define GODOTFMOD_FMOD_FILE_H

#include "classes/ref_counted.hpp"
#include "fmod.hpp"
#include <deque>

namespace godot {
    class FmodFile : public RefCounted {
        GDCLASS(FmodFile, RefCounted);

        FMOD::Sound* _wrapped = nullptr;
        std::deque<uint16_t> buffer;
        unsigned int capacity = 0;

    public:
        inline static Ref<FmodFile> create_ref(FMOD::Sound* wrapped) {
            Ref<FmodFile> ref;
            if (wrapped) {
                ref.instantiate();
                ref->_wrapped = wrapped;
                wrapped->setUserData(ref.ptr());
            }
            return ref;
        }
        

        FMOD::Sound* get_wrapped() const { return _wrapped; }

    protected:
        static void _bind_methods();
    
    public:
        
        unsigned int get_length(FMOD_TIMEUNIT lengthtype) const;
        bool release() const;
        // PackedByteArray read_data(unsigned int length) const;
        void write_data(const PackedByteArray& audio_data);
        int pop_from_buffer(void* data, unsigned int datalen);
        /* TypedArray<PackedByteArray> lock(unsigned int offset, unsigned int length) const;
        bool unlock(PackedByteArray byte_arr_1, PackedByteArray byte_arr_2) const; */
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_FILE_H
