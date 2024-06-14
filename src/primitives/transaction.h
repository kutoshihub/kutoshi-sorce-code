#ifndef BITCOIN_PRIMITIVES_TRANSACTION_H
#define BITCOIN_PRIMITIVES_TRANSACTION_H

#include "consensus/consensus.h"
#include "script/script.h"
#include "serialize.h"
#include "uint256.h"
#include "utilstrencodings.h"

enum TransactionType {
    TX_STANDARD,
    TX_SMART_CONTRACT,
};

/** An outpoint - a combination of a transaction hash and an index n into its vout */
class COutPoint
{
public:
    uint256 hash;
    uint32_t n;

    // Other existing methods...
};

/** An input of a transaction.  It contains the location of the previous
 * transaction's output that it claims and a signature that matches the
 * output's public key.
 */
class CTxIn
{
public:
    COutPoint prevout;
    CScript scriptSig;
    uint32_t nSequence;

    // Other existing methods...
};

/** An output of a transaction.  It contains the public key that the next input
 * must be able to sign with to claim it.
 */
class CTxOut
{
public:
    CAmount nValue;
    CScript scriptPubKey;

    // Other existing methods...
};

/** The basic transaction that is broadcasted on the network and contained in
 * blocks. A transaction can contain multiple inputs and outputs.
 */
class CTransaction
{
public:
    // Default transaction version.
    static const int32_t CURRENT_VERSION = 2;
    // The local variables in case of memory-only usages.
    const uint256 hash;
    // Optional. Transaction identifier.
    const uint256 m_witness_hash;

    int32_t nVersion;
    TransactionType type; // New field for transaction type
    std::vector<CTxIn> vin;
    std::vector<CTxOut> vout;
    uint32_t nLockTime;
    
    // New field for smart contract code
    std::string smartContractCode;

    // Other existing methods...

    CTransaction()
    {
        SetNull();
    }

    void SetNull()
    {
        nVersion = CTransaction::CURRENT_VERSION;
        type = TX_STANDARD; // Default to standard transaction
        vin.clear();
        vout.clear();
        nLockTime = 0;
        smartContractCode.clear(); // Initialize as empty
    }

    // Other existing methods...
};

// Serialization functions
template <typename Stream, typename TxType>
inline void SerializeTransaction(const TxType& tx, Stream& s)
{
    s << tx.nVersion;
    s << tx.type;
    s << tx.vin;
    s << tx.vout;
    s << tx.nLockTime;

    if (tx.type == TX_SMART_CONTRACT) {
        s << tx.smartContractCode;
    }
}

template <typename Stream, typename TxType>
inline void UnserializeTransaction(TxType& tx, Stream& s)
{
    s >> tx.nVersion;
    s >> tx.type;
    s >> tx.vin;
    s >> tx.vout;
    s >> tx.nLockTime;

    if (tx.type == TX_SMART_CONTRACT) {
        s >> tx.smartContractCode;
    }
}

#endif // BITCOIN_PRIMITIVES_TRANSACTION_H
