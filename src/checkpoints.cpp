// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0,   uint256("0x000000770c6aea829bb1ace7b06497f71799a6358e0e292740c4f9443a17bfb6") ) //genesis
        ( 2879,   uint256("0x675ebb9f0f934c35bbf1955565482601ef68977b2fef8248689626004fcbce2d") ) //last pow
        ( 2880,   uint256("0x3818386bc9fced59f21d5a9d1eab8a532c85efecb831eceee995ae1a7dc90419") ) //first pos
        ( 100000,   uint256("0x97b8764441d1d0eff98e834cc743ec4cd6779d53f149dec9403c9758370abd8c") )
        ( 200000,   uint256("0x98b0bcf0218293549a30d938a1d641b6bc6b498361cb432e3fafd6935c592846") )
        ( 300000,   uint256("0xdcb25c38e2731a68d5bfa16b00b8479fed57c6a352cfd94d8e0dfdd690ac9ea2") )
        ( 440000,   uint256("0x88914855018cf647d1b20162eed2dbc7cdf69757f8216868c1b55687aafe1a75") )
        ( 500000,   uint256("0x41950e8d38381ce56fc0fd9eca76e73b49ce256c01ab2521897c5c6285b39c8d") )
        ( 550000,   uint256("0x8a39262d43135792a429a4e2a6deeb669c88b618ecfa3ca855edaf1e4c3631af") )
        ( 600000,   uint256("0x23141467be3b6fe23419c8a9fbcd4df431b4b1abcd43a1fa689e7b6d137e89ef") )
        ( 619480,   uint256("0x17895abf56169141d553891d00a4c46d37023250a0089800e389bfba37e91f9a") ) // avg_fee start block
        ( 625249,   uint256("0x677def44b35e162452fb12f00c4e4fa5f1339a90a4b82753266e1cd9f5b359e7") ) // problem block on avg_fee
        ( 626372,   uint256("0x977494d951029df80bee05bdd461e6e767c916c2b60ebd356133c38ae5cf5823") ) // pre avg_fee revert block
        ( 626555,   uint256("0xab4e8bdd76283070e11377f9497d7a644d9a813fc354152b36ded76d68eb5388") ) // post avg_fee revert block
        ( 655200,   uint256("0x0dbcd2799042832cb3c1c0255e34b1ca797d30db9438d836e9e830394e98b999 ") ) // Radium 1.4.5 Code release
      ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
