#include "Distance.hpp"

Distance::Distance(Key& k1, Key& k2)
{
    const uint8_t* key1 = k1.getKey();
    const uint8_t* key2 = k2.getKey();
    
    Distance::value[ 0] = key1[ 0] ^ key2[ 0];
    Distance::value[ 1] = key1[ 1] ^ key2[ 1];
    Distance::value[ 2] = key1[ 2] ^ key2[ 2];
    Distance::value[ 3] = key1[ 3] ^ key2[ 3];
    Distance::value[ 4] = key1[ 4] ^ key2[ 4];
    Distance::value[ 5] = key1[ 5] ^ key2[ 5];
    Distance::value[ 6] = key1[ 6] ^ key2[ 6];
    Distance::value[ 7] = key1[ 7] ^ key2[ 7];
    Distance::value[ 8] = key1[ 8] ^ key2[ 8];
    Distance::value[ 9] = key1[ 9] ^ key2[ 9];
    Distance::value[10] = key1[10] ^ key2[10];
    Distance::value[11] = key1[11] ^ key2[11];
    Distance::value[12] = key1[12] ^ key2[12];
    Distance::value[13] = key1[13] ^ key2[13];
    Distance::value[14] = key1[14] ^ key2[14];
    Distance::value[15] = key1[15] ^ key2[15];
    Distance::value[16] = key1[16] ^ key2[16];
    Distance::value[17] = key1[17] ^ key2[17];
    Distance::value[18] = key1[18] ^ key2[18];
    Distance::value[19] = key1[19] ^ key2[19];
}

/*
Distance::Distance(Node& n1, Node& n2)
{
    //TODO: non so ancora come sara' la classe node
    //      suppongo ci sara' un metodo getKey() che mi ritorna la chiave
    //      della tupla, e OVVIAMENTE sara' const :)
 
    const uint8_t* key1 = n1.getKey();
    const uint8_t* key2 = n2.getKey();
    Distance::value[0] = key1[0] ^ key2[0];
    Distance::value[1] = key1[1] ^ key2[1];
    Distance::value[2] = key1[2] ^ key2[2];
    Distance::value[3] = key1[3] ^ key2[3];
    Distance::value[4] = key1[4] ^ key2[4];
    Distance::value[5] = key1[5] ^ key2[5];
    Distance::value[6] = key1[6] ^ key2[6];
    Distance::value[7] = key1[7] ^ key2[7];
    Distance::value[8] = key1[8] ^ key2[8];
    Distance::value[9] = key1[9] ^ key2[9];
    Distance::value[10] = key1[10] ^ key2[10];
    Distance::value[11] = key1[11] ^ key2[11];
    Distance::value[12] = key1[12] ^ key2[12];
    Distance::value[13] = key1[13] ^ key2[13];
    Distance::value[14] = key1[14] ^ key2[14];
    Distance::value[15] = key1[15] ^ key2[15];
    Distance::value[16] = key1[16] ^ key2[16];
    Distance::value[17] = key1[17] ^ key2[17];
    Distance::value[18] = key1[18] ^ key2[18];
    Distance::value[19] = key1[19] ^ key2[19];
}
 */

Distance::Distance()
{
    //mi serve solo per poter costruire la classe che ritorno nelle varie
    //funzioni operator+(...) senza dovergli passare per forza una key
}

Distance Distance::operator+(const Distance& k)const
{
    Distance retval;
    
    retval.value[ 0] = Distance::value[ 0] + k.value[ 0];
    retval.value[ 1] = Distance::value[ 1] + k.value[ 1];
    retval.value[ 2] = Distance::value[ 2] + k.value[ 2];
    retval.value[ 3] = Distance::value[ 3] + k.value[ 3];
    retval.value[ 4] = Distance::value[ 4] + k.value[ 4];
    retval.value[ 5] = Distance::value[ 5] + k.value[ 5];
    retval.value[ 6] = Distance::value[ 6] + k.value[ 6];
    retval.value[ 7] = Distance::value[ 7] + k.value[ 7];
    retval.value[ 8] = Distance::value[ 8] + k.value[ 8];
    retval.value[ 9] = Distance::value[ 9] + k.value[ 9];
    retval.value[10] = Distance::value[10] + k.value[10];
    retval.value[11] = Distance::value[11] + k.value[11];
    retval.value[12] = Distance::value[12] + k.value[12];
    retval.value[13] = Distance::value[13] + k.value[13];
    retval.value[14] = Distance::value[14] + k.value[14];
    retval.value[15] = Distance::value[15] + k.value[15];
    retval.value[16] = Distance::value[16] + k.value[16];
    retval.value[17] = Distance::value[17] + k.value[17];
    retval.value[18] = Distance::value[18] + k.value[18];
    retval.value[19] = Distance::value[19] + k.value[19];
    
    return retval;
}

void Distance::operator+=(const Distance& k)
{
    Distance::value[ 0] += k.value[ 0];
    Distance::value[ 1] += k.value[ 1];
    Distance::value[ 2] += k.value[ 2];
    Distance::value[ 3] += k.value[ 3];
    Distance::value[ 4] += k.value[ 4];
    Distance::value[ 5] += k.value[ 5];
    Distance::value[ 6] += k.value[ 6];
    Distance::value[ 7] += k.value[ 7];
    Distance::value[ 8] += k.value[ 8];
    Distance::value[ 9] += k.value[ 9];
    Distance::value[10] += k.value[10];
    Distance::value[11] += k.value[11];
    Distance::value[12] += k.value[12];
    Distance::value[13] += k.value[13];
    Distance::value[14] += k.value[14];
    Distance::value[15] += k.value[15];
    Distance::value[16] += k.value[16];
    Distance::value[17] += k.value[17];
    Distance::value[18] += k.value[18];
    Distance::value[19] += k.value[19];
}

Distance Distance::operator-(const Distance& k)const
{
    Distance retval;
    
    retval.value[ 0] = Distance::value[ 0] - k.value[ 0];
    retval.value[ 1] = Distance::value[ 1] - k.value[ 1];
    retval.value[ 2] = Distance::value[ 2] - k.value[ 2];
    retval.value[ 3] = Distance::value[ 3] - k.value[ 3];
    retval.value[ 4] = Distance::value[ 4] - k.value[ 4];
    retval.value[ 5] = Distance::value[ 5] - k.value[ 5];
    retval.value[ 6] = Distance::value[ 6] - k.value[ 6];
    retval.value[ 7] = Distance::value[ 7] - k.value[ 7];
    retval.value[ 8] = Distance::value[ 8] - k.value[ 8];
    retval.value[ 9] = Distance::value[ 9] - k.value[ 9];
    retval.value[10] = Distance::value[10] - k.value[10];
    retval.value[11] = Distance::value[11] - k.value[11];
    retval.value[12] = Distance::value[12] - k.value[12];
    retval.value[13] = Distance::value[13] - k.value[13];
    retval.value[14] = Distance::value[14] - k.value[14];
    retval.value[15] = Distance::value[15] - k.value[15];
    retval.value[16] = Distance::value[16] - k.value[16];
    retval.value[17] = Distance::value[17] - k.value[17];
    retval.value[18] = Distance::value[18] - k.value[18];
    retval.value[19] = Distance::value[19] - k.value[19];
    
    return retval;
}

void Distance::operator-=(const Distance& k)
{
    Distance::value[ 0] -= k.value[ 0];
    Distance::value[ 1] -= k.value[ 1];
    Distance::value[ 2] -= k.value[ 2];
    Distance::value[ 3] -= k.value[ 3];
    Distance::value[ 4] -= k.value[ 4];
    Distance::value[ 5] -= k.value[ 5];
    Distance::value[ 6] -= k.value[ 6];
    Distance::value[ 7] -= k.value[ 7];
    Distance::value[ 8] -= k.value[ 8];
    Distance::value[ 9] -= k.value[ 9];
    Distance::value[10] -= k.value[10];
    Distance::value[11] -= k.value[11];
    Distance::value[12] -= k.value[12];
    Distance::value[13] -= k.value[13];
    Distance::value[14] -= k.value[14];
    Distance::value[15] -= k.value[15];
    Distance::value[16] -= k.value[16];
    Distance::value[17] -= k.value[17];
    Distance::value[18] -= k.value[18];
    Distance::value[19] -= k.value[19];
}

bool Distance::operator<(const Distance& k)const
{
    //l'idea e' che almeno uno dei minori deve essere verificato.
    //se non lo e' allora i valori sono uguali e deve essere il prossimo.
    //se non lo e' nemmeno l'ultimo allora ciccia le chiavi sono uguali
    return
    Distance::value[ 0] < k.value[ 0] || (Distance::value[ 0]==k.value[ 0] &&
    Distance::value[ 1] < k.value[ 1] || (Distance::value[ 1]==k.value[ 1] &&
    Distance::value[ 2] < k.value[ 2] || (Distance::value[ 2]==k.value[ 2] &&
    Distance::value[ 3] < k.value[ 3] || (Distance::value[ 3]==k.value[ 3] &&
    Distance::value[ 4] < k.value[ 4] || (Distance::value[ 4]==k.value[ 4] &&
    Distance::value[ 5] < k.value[ 5] || (Distance::value[ 5]==k.value[ 5] &&
    Distance::value[ 6] < k.value[ 6] || (Distance::value[ 6]==k.value[ 6] &&
    Distance::value[ 7] < k.value[ 7] || (Distance::value[ 7]==k.value[ 7] &&
    Distance::value[ 8] < k.value[ 8] || (Distance::value[ 8]==k.value[ 8] &&
    Distance::value[ 9] < k.value[ 9] || (Distance::value[ 9]==k.value[ 9] &&
    Distance::value[10] < k.value[10] || (Distance::value[10]==k.value[10] &&
    Distance::value[11] < k.value[11] || (Distance::value[11]==k.value[11] &&
    Distance::value[12] < k.value[12] || (Distance::value[12]==k.value[12] &&
    Distance::value[13] < k.value[13] || (Distance::value[13]==k.value[13] &&
    Distance::value[14] < k.value[14] || (Distance::value[14]==k.value[14] &&
    Distance::value[15] < k.value[15] || (Distance::value[15]==k.value[15] &&
    Distance::value[16] < k.value[16] || (Distance::value[16]==k.value[16] &&
    Distance::value[17] < k.value[17] || (Distance::value[17]==k.value[17] &&
    Distance::value[18] < k.value[18] || (Distance::value[18]==k.value[18] &&
    Distance::value[19] < k.value[19])))))))))))))))))));
}

bool Distance::operator>(const Distance& k)const
{
    return k < (*this);
}

bool Distance::operator<=(const Distance& k)const
{
    return !(k > (*this));
}

bool Distance::operator>=(const Distance& k)const
{
    return !(k < (*this));
}
bool Distance::operator==(const Distance& k)const
{
    return Distance::value[ 0] == k.value[ 0] &&
           Distance::value[ 1] == k.value[ 1] &&
           Distance::value[ 2] == k.value[ 2] &&
           Distance::value[ 3] == k.value[ 3] &&
           Distance::value[ 4] == k.value[ 4] &&
           Distance::value[ 5] == k.value[ 5] &&
           Distance::value[ 6] == k.value[ 6] &&
           Distance::value[ 7] == k.value[ 7] &&
           Distance::value[ 8] == k.value[ 8] &&
           Distance::value[ 9] == k.value[ 9] &&
           Distance::value[10] == k.value[10] &&
           Distance::value[11] == k.value[11] &&
           Distance::value[12] == k.value[12] &&
           Distance::value[13] == k.value[13] &&
           Distance::value[14] == k.value[14] &&
           Distance::value[15] == k.value[15] &&
           Distance::value[16] == k.value[16] &&
           Distance::value[17] == k.value[17] &&
           Distance::value[18] == k.value[18] &&
           Distance::value[19] == k.value[19];
}
bool Distance::operator!=(const Distance& k)const
{
    return !((*this) == k);
}