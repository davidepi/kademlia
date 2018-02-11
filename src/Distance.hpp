#ifndef __DISTANCE_HPP__
#define __DISTANCE_HPP__

#include "Key.hpp"
#include "Node.hpp"
#include "settings.h"

/** \brief Class used to calculate the distance between two keys
 *
 *  In Kademlia the distance between two keys is interpreted as the exclusive
 *  or between every bit of the key. The more similar are two keys, the more
 *  near they will be. This class implements this kind of distance metric.
 */
class Distance
{
public:

    /** \brief Constructor given two keys
     *
     *  Calculate the distance between Key \p1 k1 and Key \p k2
     *
     *  \param[in] k1 The first key used to calculate the distance
     *  \param[in] k2 The second key used to calculate the distance
     */
    Distance(const Key &k1, const Key& k2);

    /** \brief Constructor given two nodes
     *
     *  Calculate the distance between Node \p n1 and Node \p n2
     *
     *  \param[in] n1 The first node used to calculate the distance
     *  \param[in] n2 The second node used to calculate the distance
     */
    Distance(Node n1, Node n2);

    ///Default destructor
    ~Distance() = default;

    /** \brief Return the distance between the two keys
     *
     *  This method calculates the distance of the two keys. The macros
     *  LEFT_DISTANCE and RIGHT_DISTANCE are used to tell whether the distance
     *  should be calculated starting from the msb or lsb first. If BMI is
     *  supported the distance is calculated with a single assembly instruction,
     *  otherwise three steps are needed. Recall that two completely different
     *  keys have maximum distance, so the distance is calculated as
     *  MAX_DISTANCE - position of the first different bit.
     *  If two keys are equal the distance is 0
     *
     *  \return The distance between the two keys, represented as a number
     *  ranging from 0 to the number of bits in the key
     */
    short getDistance()const;

    /** \brief Return a string representing the distance
     *
     *  Represent the distance with a string of the XOR distance. Every bit is
     *  represented as an ASCII char, so the string passed as input must have a
     *  size of at least 8 times the size of the key, plus one bit for the
     *  null terminating character
     *
     *  \param[out] The string representing the distance
     */
    void toString(char* out)const;

    ///True if this distance is lesser than the one passed as input
    bool operator<(const Distance&)const;
    ///True if this distance is greater than the one passed as input
    bool operator>(const Distance&)const;
    ///True if this distance is lesser than or equal the one passed as input
    bool operator<=(const Distance&)const;
    ///True if this distance is greater than or equal the one passed as input
    bool operator>=(const Distance&)const;
    ///True if the distance is equal to another one
    bool operator==(const Distance&)const;
    ///True if the distance is different from another one
    bool operator!=(const Distance&)const;

private:

    uint8_t value[20];

};

#endif
