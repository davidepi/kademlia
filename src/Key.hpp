#ifndef __KEY_HPP__
#define __KEY_HPP__

#include <cstdlib> //uint32_t
#include <climits> //CHAR_BIT
#include <cstdio>
#include <sstream>
#include <functional>
#include "Ip.hpp"
#include "settings.h"

/** \brief Class used to define an unique key
 *
 *  This class generates an hashed key given some bytes, or Ip and a port.
 *  The underlying algorithm used for key generation is SHA-1
 */
class Key
{
public:

    /** \brief Constructor given ip, port
     *
     *  Generates a Key given the Ip and port of a node. Port is expected to be
     *  in network order. Although this does not change much by means of the
     *  key generation, it is required to be in network order to mantain
     *  consistency with the keys (Machine with different endianness must see
     *  the same pair <Ip,port> with the same key)
     *
     *  \param[in] The Ip of the node that will be used for the key
     *  \param[in] The port of the node that will be used for the key, in
     *  network order
     */
    Key(Ip ip, uint16_t port_no);

    /** \brief Constructor given c-string
     *
     *  Generates a Key given a null-terminated c-string
     *
     *  \param[in] name The input string
     */
    Key(const char* name);

    /** \brief Default constructor
     *
     *  The key is left uninitialized, a subsequent call to Key::craft() is
     *  necessary. Useful if the key was received from another host and thus
     *  it won't be calculated but reconstructed from a byte array.
     *
     *  \sa craft();
     */
    Key() = default;

    ///Default destructor
    ~Key() = default;

    /** \brief Return a constant version of the key
     *
     *  Return this key as read-only to be accessed by other classes like the
     *  Distance. Internally the key is represented as a 20 bytes array, and
     *  a const pointer to this array is returned
     *
     *  \return A const pointer to the underlying array containing the key
     */
    const uint8_t* getKey()const;

    /** \brief Generates a key without calculating the hash
     *
     *  Given a 20 bytes array, construct the class by copying these values.
     *  This differs from the Key(const char* name) constructor because the
     *  key here it is provided as is, and the input value are not hashed like
     *  in the aforementioned constructor. Useful if the key was received from
     *  another host.
     *
     *  \param[in] bytes A 20 bytes array containing the key that will be copied
     *  into this class
     *  \sa Key();
     *  \sa Key(const char*);
     */
    void craft(const uint8_t* bytes);

    ///Print the key to stdout, as an hexadecimal string
    void print()const;

    ///True if this key is equal to the one passed as input
    bool operator==(const Key& k)const;
    ///True if this key is different from the one passed as input
    bool operator!=(const Key& k)const;
    ///Used in order to hash this key
    std::size_t operator()(const Key& c) const;

private:

    uint8_t key[NBYTE];
};

//used to print the string with std::cout
std::ostream& operator<<(std::ostream& strm, const Key& k);

//used for the hash
namespace std
{
    template <> struct hash<Key>
    {
        size_t operator()(const Key & k) const
        {
            using std::hash;
            return ((hash<uint8_t>()(k.getKey()[ 0]) >>  1) ^
                    (hash<uint8_t>()(k.getKey()[ 1]) >>  2) ^
                    (hash<uint8_t>()(k.getKey()[ 2]) >>  3) ^
                    (hash<uint8_t>()(k.getKey()[ 3]) >>  4) ^
                    (hash<uint8_t>()(k.getKey()[ 4]) >>  5) ^
                    (hash<uint8_t>()(k.getKey()[ 5]) >>  6) ^
                    (hash<uint8_t>()(k.getKey()[ 6]) >>  7) ^
                    (hash<uint8_t>()(k.getKey()[ 7]) >>  8) ^
                    (hash<uint8_t>()(k.getKey()[ 8]) >>  9) ^
                    (hash<uint8_t>()(k.getKey()[ 9]) >> 10) ^
                    (hash<uint8_t>()(k.getKey()[10]) >> 11) ^
                    (hash<uint8_t>()(k.getKey()[11]) >> 12) ^
                    (hash<uint8_t>()(k.getKey()[12]) >> 13) ^
                    (hash<uint8_t>()(k.getKey()[13]) >> 14) ^
                    (hash<uint8_t>()(k.getKey()[14]) >> 15) ^
                    (hash<uint8_t>()(k.getKey()[15]) >> 16) ^
                    (hash<uint8_t>()(k.getKey()[16]) >> 17) ^
                    (hash<uint8_t>()(k.getKey()[17]) >> 18) ^
                    (hash<uint8_t>()(k.getKey()[18]) >> 19) ^
                    (hash<uint8_t>()(k.getKey()[19]) >> 20));
        }
    };
}

#endif

