cd /MP-SPDZ
./compile.py topk
./semi-party.x -N $NUM_PARTIES -I -p $PARTY_ID topk
