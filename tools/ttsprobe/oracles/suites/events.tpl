@open
buf 0 64
buf 1 64
echo -- ENG_ORDR
@load ENG_ORDR.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.w SEQDESTROYSEQ 0001
echo -- ENG_PARM
@load ENG_PARM.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
echo stream 0101
call.d SEQSTREAMLEN 0101
call.d SEQSEEK 0101 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
echo stream 0102
call.d SEQSTREAMLEN 0102
call.d SEQSEEK 0102 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
echo stream 0103
call.d SEQSTREAMLEN 0103
call.d SEQSEEK 0103 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
echo stream 0104
call.d SEQSTREAMLEN 0104
call.d SEQSEEK 0104 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
echo stream 0105
call.d SEQSTREAMLEN 0105
call.d SEQSEEK 0105 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.d SEQEVTATTIMEGE 0101 d:00000000
call.d SEQEVTATTIMEGT 0101 d:00000000
call.d SEQEVTATTIMEGE 0101 d:00000001
call.d SEQEVTATTIMEGT 0101 d:00000001
call.d SEQEVTATTIMEGE 0101 d:00000077
call.d SEQEVTATTIMEGT 0101 d:00000077
call.d SEQEVTATTIMEGE 0101 d:00000078
call.d SEQEVTATTIMEGT 0101 d:00000078
call.d SEQEVTATTIMEGE 0101 d:000000f0
call.d SEQEVTATTIMEGT 0101 d:000000f0
call.d SEQEVTATTIMEGE 0101 d:000001e0
call.d SEQEVTATTIMEGT 0101 d:000001e0
call.d SEQEVTATTIMEGE 0101 d:000001df
call.d SEQEVTATTIMEGT 0101 d:000001df
call.d SEQEVTATTIMEGE 0101 d:000003c0
call.d SEQEVTATTIMEGT 0101 d:000003c0
call.d SEQEVTATTIMEGE 0101 d:00010000
call.d SEQEVTATTIMEGT 0101 d:00010000
call.d SEQSTREAMDUR 0101
call.w SEQMEASHASEVTS 0101 0001
call.w SEQMEASHASEVTS 0101 0002
call.w SEQMEASHASEVTS 0101 0003
call.w SEQMEASHASEVTS 0101 0004
call.w SEQMEASHASEVTS 0101 0005
call.w SEQMEASHASEVTS 0101 0006
call.w SEQMEASHASEVTS 0101 0007
call.w SEQMEASHASEVTS 0101 0008
call.w SEQMEASHASEVTS 0101 0009
call.w SEQMEASHASEVTS 0101 000a
call.w SEQMEASHASEVTS 0101 000b
call.d SEQEVTATTIMEGE 0102 d:00000000
call.d SEQEVTATTIMEGT 0102 d:00000000
call.d SEQEVTATTIMEGE 0102 d:00000001
call.d SEQEVTATTIMEGT 0102 d:00000001
call.d SEQEVTATTIMEGE 0102 d:00000077
call.d SEQEVTATTIMEGT 0102 d:00000077
call.d SEQEVTATTIMEGE 0102 d:00000078
call.d SEQEVTATTIMEGT 0102 d:00000078
call.d SEQEVTATTIMEGE 0102 d:000000f0
call.d SEQEVTATTIMEGT 0102 d:000000f0
call.d SEQEVTATTIMEGE 0102 d:000001e0
call.d SEQEVTATTIMEGT 0102 d:000001e0
call.d SEQEVTATTIMEGE 0102 d:000001df
call.d SEQEVTATTIMEGT 0102 d:000001df
call.d SEQEVTATTIMEGE 0102 d:000003c0
call.d SEQEVTATTIMEGT 0102 d:000003c0
call.d SEQEVTATTIMEGE 0102 d:00010000
call.d SEQEVTATTIMEGT 0102 d:00010000
call.d SEQSTREAMDUR 0102
call.w SEQMEASHASEVTS 0102 0001
call.w SEQMEASHASEVTS 0102 0002
call.w SEQMEASHASEVTS 0102 0003
call.w SEQMEASHASEVTS 0102 0004
call.w SEQMEASHASEVTS 0102 0005
call.w SEQMEASHASEVTS 0102 0006
call.w SEQMEASHASEVTS 0102 0007
call.w SEQMEASHASEVTS 0102 0008
call.w SEQMEASHASEVTS 0102 0009
call.w SEQMEASHASEVTS 0102 000a
call.w SEQMEASHASEVTS 0102 000b
call.d SEQEVTATTIMEGE 0103 d:00000000
call.d SEQEVTATTIMEGT 0103 d:00000000
call.d SEQEVTATTIMEGE 0103 d:00000001
call.d SEQEVTATTIMEGT 0103 d:00000001
call.d SEQEVTATTIMEGE 0103 d:00000077
call.d SEQEVTATTIMEGT 0103 d:00000077
call.d SEQEVTATTIMEGE 0103 d:00000078
call.d SEQEVTATTIMEGT 0103 d:00000078
call.d SEQEVTATTIMEGE 0103 d:000000f0
call.d SEQEVTATTIMEGT 0103 d:000000f0
call.d SEQEVTATTIMEGE 0103 d:000001e0
call.d SEQEVTATTIMEGT 0103 d:000001e0
call.d SEQEVTATTIMEGE 0103 d:000001df
call.d SEQEVTATTIMEGT 0103 d:000001df
call.d SEQEVTATTIMEGE 0103 d:000003c0
call.d SEQEVTATTIMEGT 0103 d:000003c0
call.d SEQEVTATTIMEGE 0103 d:00010000
call.d SEQEVTATTIMEGT 0103 d:00010000
call.d SEQSTREAMDUR 0103
call.w SEQMEASHASEVTS 0103 0001
call.w SEQMEASHASEVTS 0103 0002
call.w SEQMEASHASEVTS 0103 0003
call.w SEQMEASHASEVTS 0103 0004
call.w SEQMEASHASEVTS 0103 0005
call.w SEQMEASHASEVTS 0103 0006
call.w SEQMEASHASEVTS 0103 0007
call.w SEQMEASHASEVTS 0103 0008
call.w SEQMEASHASEVTS 0103 0009
call.w SEQMEASHASEVTS 0103 000a
call.w SEQMEASHASEVTS 0103 000b
call.d SEQEVTATTIMEGE 0104 d:00000000
call.d SEQEVTATTIMEGT 0104 d:00000000
call.d SEQEVTATTIMEGE 0104 d:00000001
call.d SEQEVTATTIMEGT 0104 d:00000001
call.d SEQEVTATTIMEGE 0104 d:00000077
call.d SEQEVTATTIMEGT 0104 d:00000077
call.d SEQEVTATTIMEGE 0104 d:00000078
call.d SEQEVTATTIMEGT 0104 d:00000078
call.d SEQEVTATTIMEGE 0104 d:000000f0
call.d SEQEVTATTIMEGT 0104 d:000000f0
call.d SEQEVTATTIMEGE 0104 d:000001e0
call.d SEQEVTATTIMEGT 0104 d:000001e0
call.d SEQEVTATTIMEGE 0104 d:000001df
call.d SEQEVTATTIMEGT 0104 d:000001df
call.d SEQEVTATTIMEGE 0104 d:000003c0
call.d SEQEVTATTIMEGT 0104 d:000003c0
call.d SEQEVTATTIMEGE 0104 d:00010000
call.d SEQEVTATTIMEGT 0104 d:00010000
call.d SEQSTREAMDUR 0104
call.w SEQMEASHASEVTS 0104 0001
call.w SEQMEASHASEVTS 0104 0002
call.w SEQMEASHASEVTS 0104 0003
call.w SEQMEASHASEVTS 0104 0004
call.w SEQMEASHASEVTS 0104 0005
call.w SEQMEASHASEVTS 0104 0006
call.w SEQMEASHASEVTS 0104 0007
call.w SEQMEASHASEVTS 0104 0008
call.w SEQMEASHASEVTS 0104 0009
call.w SEQMEASHASEVTS 0104 000a
call.w SEQMEASHASEVTS 0104 000b
call.d SEQEVTATTIMEGE 0105 d:00000000
call.d SEQEVTATTIMEGT 0105 d:00000000
call.d SEQEVTATTIMEGE 0105 d:00000001
call.d SEQEVTATTIMEGT 0105 d:00000001
call.d SEQEVTATTIMEGE 0105 d:00000077
call.d SEQEVTATTIMEGT 0105 d:00000077
call.d SEQEVTATTIMEGE 0105 d:00000078
call.d SEQEVTATTIMEGT 0105 d:00000078
call.d SEQEVTATTIMEGE 0105 d:000000f0
call.d SEQEVTATTIMEGT 0105 d:000000f0
call.d SEQEVTATTIMEGE 0105 d:000001e0
call.d SEQEVTATTIMEGT 0105 d:000001e0
call.d SEQEVTATTIMEGE 0105 d:000001df
call.d SEQEVTATTIMEGT 0105 d:000001df
call.d SEQEVTATTIMEGE 0105 d:000003c0
call.d SEQEVTATTIMEGT 0105 d:000003c0
call.d SEQEVTATTIMEGE 0105 d:00010000
call.d SEQEVTATTIMEGT 0105 d:00010000
call.d SEQSTREAMDUR 0105
call.w SEQMEASHASEVTS 0105 0001
call.w SEQMEASHASEVTS 0105 0002
call.w SEQMEASHASEVTS 0105 0003
call.w SEQMEASHASEVTS 0105 0004
call.w SEQMEASHASEVTS 0105 0005
call.w SEQMEASHASEVTS 0105 0006
call.w SEQMEASHASEVTS 0105 0007
call.w SEQMEASHASEVTS 0105 0008
call.w SEQMEASHASEVTS 0105 0009
call.w SEQMEASHASEVTS 0105 000a
call.w SEQMEASHASEVTS 0105 000b
call.w SEQDESTROYSEQ 0001
echo -- SYN_KIND
@load SYN_KIND.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
echo stream 0101
call.d SEQSTREAMLEN 0101
call.d SEQSEEK 0101 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.d SEQEVTATTIMEGE 0101 d:00000000
call.d SEQEVTATTIMEGT 0101 d:00000000
call.d SEQEVTATTIMEGE 0101 d:00000001
call.d SEQEVTATTIMEGT 0101 d:00000001
call.d SEQEVTATTIMEGE 0101 d:00000077
call.d SEQEVTATTIMEGT 0101 d:00000077
call.d SEQEVTATTIMEGE 0101 d:00000078
call.d SEQEVTATTIMEGT 0101 d:00000078
call.d SEQEVTATTIMEGE 0101 d:000000f0
call.d SEQEVTATTIMEGT 0101 d:000000f0
call.d SEQEVTATTIMEGE 0101 d:000001e0
call.d SEQEVTATTIMEGT 0101 d:000001e0
call.d SEQEVTATTIMEGE 0101 d:000001df
call.d SEQEVTATTIMEGT 0101 d:000001df
call.d SEQEVTATTIMEGE 0101 d:000003c0
call.d SEQEVTATTIMEGT 0101 d:000003c0
call.d SEQEVTATTIMEGE 0101 d:00010000
call.d SEQEVTATTIMEGT 0101 d:00010000
call.d SEQSTREAMDUR 0101
call.w SEQMEASHASEVTS 0101 0001
call.w SEQMEASHASEVTS 0101 0002
call.w SEQMEASHASEVTS 0101 0003
call.w SEQMEASHASEVTS 0101 0004
call.w SEQMEASHASEVTS 0101 0005
call.w SEQMEASHASEVTS 0101 0006
call.w SEQMEASHASEVTS 0101 0007
call.w SEQMEASHASEVTS 0101 0008
call.w SEQMEASHASEVTS 0101 0009
call.w SEQMEASHASEVTS 0101 000a
call.w SEQMEASHASEVTS 0101 000b
call.w SEQDESTROYSEQ 0001
echo -- SYN_MIN
@load SYN_MIN.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.w SEQDESTROYSEQ 0001
echo -- SYN_OVLP
@load SYN_OVLP.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
echo stream 0101
call.d SEQSTREAMLEN 0101
call.d SEQSEEK 0101 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.d SEQEVTATTIMEGE 0101 d:00000000
call.d SEQEVTATTIMEGT 0101 d:00000000
call.d SEQEVTATTIMEGE 0101 d:00000001
call.d SEQEVTATTIMEGT 0101 d:00000001
call.d SEQEVTATTIMEGE 0101 d:00000077
call.d SEQEVTATTIMEGT 0101 d:00000077
call.d SEQEVTATTIMEGE 0101 d:00000078
call.d SEQEVTATTIMEGT 0101 d:00000078
call.d SEQEVTATTIMEGE 0101 d:000000f0
call.d SEQEVTATTIMEGT 0101 d:000000f0
call.d SEQEVTATTIMEGE 0101 d:000001e0
call.d SEQEVTATTIMEGT 0101 d:000001e0
call.d SEQEVTATTIMEGE 0101 d:000001df
call.d SEQEVTATTIMEGT 0101 d:000001df
call.d SEQEVTATTIMEGE 0101 d:000003c0
call.d SEQEVTATTIMEGT 0101 d:000003c0
call.d SEQEVTATTIMEGE 0101 d:00010000
call.d SEQEVTATTIMEGT 0101 d:00010000
call.d SEQSTREAMDUR 0101
call.w SEQMEASHASEVTS 0101 0001
call.w SEQMEASHASEVTS 0101 0002
call.w SEQMEASHASEVTS 0101 0003
call.w SEQMEASHASEVTS 0101 0004
call.w SEQMEASHASEVTS 0101 0005
call.w SEQMEASHASEVTS 0101 0006
call.w SEQMEASHASEVTS 0101 0007
call.w SEQMEASHASEVTS 0101 0008
call.w SEQMEASHASEVTS 0101 0009
call.w SEQMEASHASEVTS 0101 000a
call.w SEQMEASHASEVTS 0101 000b
call.w SEQDESTROYSEQ 0001
echo -- XPOSE
@load XPOSE.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
echo stream 0101
call.d SEQSTREAMLEN 0101
call.d SEQSEEK 0101 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
echo stream 0102
call.d SEQSTREAMLEN 0102
call.d SEQSEEK 0102 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.d SEQEVTATTIMEGE 0101 d:00000000
call.d SEQEVTATTIMEGT 0101 d:00000000
call.d SEQEVTATTIMEGE 0101 d:00000001
call.d SEQEVTATTIMEGT 0101 d:00000001
call.d SEQEVTATTIMEGE 0101 d:00000077
call.d SEQEVTATTIMEGT 0101 d:00000077
call.d SEQEVTATTIMEGE 0101 d:00000078
call.d SEQEVTATTIMEGT 0101 d:00000078
call.d SEQEVTATTIMEGE 0101 d:000000f0
call.d SEQEVTATTIMEGT 0101 d:000000f0
call.d SEQEVTATTIMEGE 0101 d:000001e0
call.d SEQEVTATTIMEGT 0101 d:000001e0
call.d SEQEVTATTIMEGE 0101 d:000001df
call.d SEQEVTATTIMEGT 0101 d:000001df
call.d SEQEVTATTIMEGE 0101 d:000003c0
call.d SEQEVTATTIMEGT 0101 d:000003c0
call.d SEQEVTATTIMEGE 0101 d:00010000
call.d SEQEVTATTIMEGT 0101 d:00010000
call.d SEQSTREAMDUR 0101
call.w SEQMEASHASEVTS 0101 0001
call.w SEQMEASHASEVTS 0101 0002
call.w SEQMEASHASEVTS 0101 0003
call.w SEQMEASHASEVTS 0101 0004
call.w SEQMEASHASEVTS 0101 0005
call.w SEQMEASHASEVTS 0101 0006
call.w SEQMEASHASEVTS 0101 0007
call.w SEQMEASHASEVTS 0101 0008
call.w SEQMEASHASEVTS 0101 0009
call.w SEQMEASHASEVTS 0101 000a
call.w SEQMEASHASEVTS 0101 000b
call.d SEQEVTATTIMEGE 0102 d:00000000
call.d SEQEVTATTIMEGT 0102 d:00000000
call.d SEQEVTATTIMEGE 0102 d:00000001
call.d SEQEVTATTIMEGT 0102 d:00000001
call.d SEQEVTATTIMEGE 0102 d:00000077
call.d SEQEVTATTIMEGT 0102 d:00000077
call.d SEQEVTATTIMEGE 0102 d:00000078
call.d SEQEVTATTIMEGT 0102 d:00000078
call.d SEQEVTATTIMEGE 0102 d:000000f0
call.d SEQEVTATTIMEGT 0102 d:000000f0
call.d SEQEVTATTIMEGE 0102 d:000001e0
call.d SEQEVTATTIMEGT 0102 d:000001e0
call.d SEQEVTATTIMEGE 0102 d:000001df
call.d SEQEVTATTIMEGT 0102 d:000001df
call.d SEQEVTATTIMEGE 0102 d:000003c0
call.d SEQEVTATTIMEGT 0102 d:000003c0
call.d SEQEVTATTIMEGE 0102 d:00010000
call.d SEQEVTATTIMEGT 0102 d:00010000
call.d SEQSTREAMDUR 0102
call.w SEQMEASHASEVTS 0102 0001
call.w SEQMEASHASEVTS 0102 0002
call.w SEQMEASHASEVTS 0102 0003
call.w SEQMEASHASEVTS 0102 0004
call.w SEQMEASHASEVTS 0102 0005
call.w SEQMEASHASEVTS 0102 0006
call.w SEQMEASHASEVTS 0102 0007
call.w SEQMEASHASEVTS 0102 0008
call.w SEQMEASHASEVTS 0102 0009
call.w SEQMEASHASEVTS 0102 000a
call.w SEQMEASHASEVTS 0102 000b
call.w SEQDESTROYSEQ 0001
echo -- EFILT
@load EFILT.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
echo stream 0101
call.d SEQSTREAMLEN 0101
call.d SEQSEEK 0101 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0101 @0
dump 0 0 8
call.d SEQTELL 0101
call.w SEQEOS 0101
call.w SEQNXTEVT 0101
echo stream 0102
call.d SEQSTREAMLEN 0102
call.d SEQSEEK 0102 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0102 @0
dump 0 0 8
call.d SEQTELL 0102
call.w SEQEOS 0102
call.w SEQNXTEVT 0102
echo stream 0103
call.d SEQSTREAMLEN 0103
call.d SEQSEEK 0103 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0103 @0
dump 0 0 8
call.d SEQTELL 0103
call.w SEQEOS 0103
call.w SEQNXTEVT 0103
echo stream 0104
call.d SEQSTREAMLEN 0104
call.d SEQSEEK 0104 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0104 @0
dump 0 0 8
call.d SEQTELL 0104
call.w SEQEOS 0104
call.w SEQNXTEVT 0104
echo stream 0105
call.d SEQSTREAMLEN 0105
call.d SEQSEEK 0105 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0105 @0
dump 0 0 8
call.d SEQTELL 0105
call.w SEQEOS 0105
call.w SEQNXTEVT 0105
echo stream 0106
call.d SEQSTREAMLEN 0106
call.d SEQSEEK 0106 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0106 @0
dump 0 0 8
call.d SEQTELL 0106
call.w SEQEOS 0106
call.w SEQNXTEVT 0106
echo stream 0107
call.d SEQSTREAMLEN 0107
call.d SEQSEEK 0107 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0107 @0
dump 0 0 8
call.d SEQTELL 0107
call.w SEQEOS 0107
call.w SEQNXTEVT 0107
echo stream 0108
call.d SEQSTREAMLEN 0108
call.d SEQSEEK 0108 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0108 @0
dump 0 0 8
call.d SEQTELL 0108
call.w SEQEOS 0108
call.w SEQNXTEVT 0108
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.d SEQEVTATTIMEGE 0101 d:00000000
call.d SEQEVTATTIMEGT 0101 d:00000000
call.d SEQEVTATTIMEGE 0101 d:00000001
call.d SEQEVTATTIMEGT 0101 d:00000001
call.d SEQEVTATTIMEGE 0101 d:00000077
call.d SEQEVTATTIMEGT 0101 d:00000077
call.d SEQEVTATTIMEGE 0101 d:00000078
call.d SEQEVTATTIMEGT 0101 d:00000078
call.d SEQEVTATTIMEGE 0101 d:000000f0
call.d SEQEVTATTIMEGT 0101 d:000000f0
call.d SEQEVTATTIMEGE 0101 d:000001e0
call.d SEQEVTATTIMEGT 0101 d:000001e0
call.d SEQEVTATTIMEGE 0101 d:000001df
call.d SEQEVTATTIMEGT 0101 d:000001df
call.d SEQEVTATTIMEGE 0101 d:000003c0
call.d SEQEVTATTIMEGT 0101 d:000003c0
call.d SEQEVTATTIMEGE 0101 d:00010000
call.d SEQEVTATTIMEGT 0101 d:00010000
call.d SEQSTREAMDUR 0101
call.w SEQMEASHASEVTS 0101 0001
call.w SEQMEASHASEVTS 0101 0002
call.w SEQMEASHASEVTS 0101 0003
call.w SEQMEASHASEVTS 0101 0004
call.w SEQMEASHASEVTS 0101 0005
call.w SEQMEASHASEVTS 0101 0006
call.w SEQMEASHASEVTS 0101 0007
call.w SEQMEASHASEVTS 0101 0008
call.w SEQMEASHASEVTS 0101 0009
call.w SEQMEASHASEVTS 0101 000a
call.w SEQMEASHASEVTS 0101 000b
call.d SEQEVTATTIMEGE 0102 d:00000000
call.d SEQEVTATTIMEGT 0102 d:00000000
call.d SEQEVTATTIMEGE 0102 d:00000001
call.d SEQEVTATTIMEGT 0102 d:00000001
call.d SEQEVTATTIMEGE 0102 d:00000077
call.d SEQEVTATTIMEGT 0102 d:00000077
call.d SEQEVTATTIMEGE 0102 d:00000078
call.d SEQEVTATTIMEGT 0102 d:00000078
call.d SEQEVTATTIMEGE 0102 d:000000f0
call.d SEQEVTATTIMEGT 0102 d:000000f0
call.d SEQEVTATTIMEGE 0102 d:000001e0
call.d SEQEVTATTIMEGT 0102 d:000001e0
call.d SEQEVTATTIMEGE 0102 d:000001df
call.d SEQEVTATTIMEGT 0102 d:000001df
call.d SEQEVTATTIMEGE 0102 d:000003c0
call.d SEQEVTATTIMEGT 0102 d:000003c0
call.d SEQEVTATTIMEGE 0102 d:00010000
call.d SEQEVTATTIMEGT 0102 d:00010000
call.d SEQSTREAMDUR 0102
call.w SEQMEASHASEVTS 0102 0001
call.w SEQMEASHASEVTS 0102 0002
call.w SEQMEASHASEVTS 0102 0003
call.w SEQMEASHASEVTS 0102 0004
call.w SEQMEASHASEVTS 0102 0005
call.w SEQMEASHASEVTS 0102 0006
call.w SEQMEASHASEVTS 0102 0007
call.w SEQMEASHASEVTS 0102 0008
call.w SEQMEASHASEVTS 0102 0009
call.w SEQMEASHASEVTS 0102 000a
call.w SEQMEASHASEVTS 0102 000b
call.d SEQEVTATTIMEGE 0103 d:00000000
call.d SEQEVTATTIMEGT 0103 d:00000000
call.d SEQEVTATTIMEGE 0103 d:00000001
call.d SEQEVTATTIMEGT 0103 d:00000001
call.d SEQEVTATTIMEGE 0103 d:00000077
call.d SEQEVTATTIMEGT 0103 d:00000077
call.d SEQEVTATTIMEGE 0103 d:00000078
call.d SEQEVTATTIMEGT 0103 d:00000078
call.d SEQEVTATTIMEGE 0103 d:000000f0
call.d SEQEVTATTIMEGT 0103 d:000000f0
call.d SEQEVTATTIMEGE 0103 d:000001e0
call.d SEQEVTATTIMEGT 0103 d:000001e0
call.d SEQEVTATTIMEGE 0103 d:000001df
call.d SEQEVTATTIMEGT 0103 d:000001df
call.d SEQEVTATTIMEGE 0103 d:000003c0
call.d SEQEVTATTIMEGT 0103 d:000003c0
call.d SEQEVTATTIMEGE 0103 d:00010000
call.d SEQEVTATTIMEGT 0103 d:00010000
call.d SEQSTREAMDUR 0103
call.w SEQMEASHASEVTS 0103 0001
call.w SEQMEASHASEVTS 0103 0002
call.w SEQMEASHASEVTS 0103 0003
call.w SEQMEASHASEVTS 0103 0004
call.w SEQMEASHASEVTS 0103 0005
call.w SEQMEASHASEVTS 0103 0006
call.w SEQMEASHASEVTS 0103 0007
call.w SEQMEASHASEVTS 0103 0008
call.w SEQMEASHASEVTS 0103 0009
call.w SEQMEASHASEVTS 0103 000a
call.w SEQMEASHASEVTS 0103 000b
call.d SEQEVTATTIMEGE 0104 d:00000000
call.d SEQEVTATTIMEGT 0104 d:00000000
call.d SEQEVTATTIMEGE 0104 d:00000001
call.d SEQEVTATTIMEGT 0104 d:00000001
call.d SEQEVTATTIMEGE 0104 d:00000077
call.d SEQEVTATTIMEGT 0104 d:00000077
call.d SEQEVTATTIMEGE 0104 d:00000078
call.d SEQEVTATTIMEGT 0104 d:00000078
call.d SEQEVTATTIMEGE 0104 d:000000f0
call.d SEQEVTATTIMEGT 0104 d:000000f0
call.d SEQEVTATTIMEGE 0104 d:000001e0
call.d SEQEVTATTIMEGT 0104 d:000001e0
call.d SEQEVTATTIMEGE 0104 d:000001df
call.d SEQEVTATTIMEGT 0104 d:000001df
call.d SEQEVTATTIMEGE 0104 d:000003c0
call.d SEQEVTATTIMEGT 0104 d:000003c0
call.d SEQEVTATTIMEGE 0104 d:00010000
call.d SEQEVTATTIMEGT 0104 d:00010000
call.d SEQSTREAMDUR 0104
call.w SEQMEASHASEVTS 0104 0001
call.w SEQMEASHASEVTS 0104 0002
call.w SEQMEASHASEVTS 0104 0003
call.w SEQMEASHASEVTS 0104 0004
call.w SEQMEASHASEVTS 0104 0005
call.w SEQMEASHASEVTS 0104 0006
call.w SEQMEASHASEVTS 0104 0007
call.w SEQMEASHASEVTS 0104 0008
call.w SEQMEASHASEVTS 0104 0009
call.w SEQMEASHASEVTS 0104 000a
call.w SEQMEASHASEVTS 0104 000b
call.d SEQEVTATTIMEGE 0105 d:00000000
call.d SEQEVTATTIMEGT 0105 d:00000000
call.d SEQEVTATTIMEGE 0105 d:00000001
call.d SEQEVTATTIMEGT 0105 d:00000001
call.d SEQEVTATTIMEGE 0105 d:00000077
call.d SEQEVTATTIMEGT 0105 d:00000077
call.d SEQEVTATTIMEGE 0105 d:00000078
call.d SEQEVTATTIMEGT 0105 d:00000078
call.d SEQEVTATTIMEGE 0105 d:000000f0
call.d SEQEVTATTIMEGT 0105 d:000000f0
call.d SEQEVTATTIMEGE 0105 d:000001e0
call.d SEQEVTATTIMEGT 0105 d:000001e0
call.d SEQEVTATTIMEGE 0105 d:000001df
call.d SEQEVTATTIMEGT 0105 d:000001df
call.d SEQEVTATTIMEGE 0105 d:000003c0
call.d SEQEVTATTIMEGT 0105 d:000003c0
call.d SEQEVTATTIMEGE 0105 d:00010000
call.d SEQEVTATTIMEGT 0105 d:00010000
call.d SEQSTREAMDUR 0105
call.w SEQMEASHASEVTS 0105 0001
call.w SEQMEASHASEVTS 0105 0002
call.w SEQMEASHASEVTS 0105 0003
call.w SEQMEASHASEVTS 0105 0004
call.w SEQMEASHASEVTS 0105 0005
call.w SEQMEASHASEVTS 0105 0006
call.w SEQMEASHASEVTS 0105 0007
call.w SEQMEASHASEVTS 0105 0008
call.w SEQMEASHASEVTS 0105 0009
call.w SEQMEASHASEVTS 0105 000a
call.w SEQMEASHASEVTS 0105 000b
call.d SEQEVTATTIMEGE 0106 d:00000000
call.d SEQEVTATTIMEGT 0106 d:00000000
call.d SEQEVTATTIMEGE 0106 d:00000001
call.d SEQEVTATTIMEGT 0106 d:00000001
call.d SEQEVTATTIMEGE 0106 d:00000077
call.d SEQEVTATTIMEGT 0106 d:00000077
call.d SEQEVTATTIMEGE 0106 d:00000078
call.d SEQEVTATTIMEGT 0106 d:00000078
call.d SEQEVTATTIMEGE 0106 d:000000f0
call.d SEQEVTATTIMEGT 0106 d:000000f0
call.d SEQEVTATTIMEGE 0106 d:000001e0
call.d SEQEVTATTIMEGT 0106 d:000001e0
call.d SEQEVTATTIMEGE 0106 d:000001df
call.d SEQEVTATTIMEGT 0106 d:000001df
call.d SEQEVTATTIMEGE 0106 d:000003c0
call.d SEQEVTATTIMEGT 0106 d:000003c0
call.d SEQEVTATTIMEGE 0106 d:00010000
call.d SEQEVTATTIMEGT 0106 d:00010000
call.d SEQSTREAMDUR 0106
call.w SEQMEASHASEVTS 0106 0001
call.w SEQMEASHASEVTS 0106 0002
call.w SEQMEASHASEVTS 0106 0003
call.w SEQMEASHASEVTS 0106 0004
call.w SEQMEASHASEVTS 0106 0005
call.w SEQMEASHASEVTS 0106 0006
call.w SEQMEASHASEVTS 0106 0007
call.w SEQMEASHASEVTS 0106 0008
call.w SEQMEASHASEVTS 0106 0009
call.w SEQMEASHASEVTS 0106 000a
call.w SEQMEASHASEVTS 0106 000b
call.d SEQEVTATTIMEGE 0107 d:00000000
call.d SEQEVTATTIMEGT 0107 d:00000000
call.d SEQEVTATTIMEGE 0107 d:00000001
call.d SEQEVTATTIMEGT 0107 d:00000001
call.d SEQEVTATTIMEGE 0107 d:00000077
call.d SEQEVTATTIMEGT 0107 d:00000077
call.d SEQEVTATTIMEGE 0107 d:00000078
call.d SEQEVTATTIMEGT 0107 d:00000078
call.d SEQEVTATTIMEGE 0107 d:000000f0
call.d SEQEVTATTIMEGT 0107 d:000000f0
call.d SEQEVTATTIMEGE 0107 d:000001e0
call.d SEQEVTATTIMEGT 0107 d:000001e0
call.d SEQEVTATTIMEGE 0107 d:000001df
call.d SEQEVTATTIMEGT 0107 d:000001df
call.d SEQEVTATTIMEGE 0107 d:000003c0
call.d SEQEVTATTIMEGT 0107 d:000003c0
call.d SEQEVTATTIMEGE 0107 d:00010000
call.d SEQEVTATTIMEGT 0107 d:00010000
call.d SEQSTREAMDUR 0107
call.w SEQMEASHASEVTS 0107 0001
call.w SEQMEASHASEVTS 0107 0002
call.w SEQMEASHASEVTS 0107 0003
call.w SEQMEASHASEVTS 0107 0004
call.w SEQMEASHASEVTS 0107 0005
call.w SEQMEASHASEVTS 0107 0006
call.w SEQMEASHASEVTS 0107 0007
call.w SEQMEASHASEVTS 0107 0008
call.w SEQMEASHASEVTS 0107 0009
call.w SEQMEASHASEVTS 0107 000a
call.w SEQMEASHASEVTS 0107 000b
call.d SEQEVTATTIMEGE 0108 d:00000000
call.d SEQEVTATTIMEGT 0108 d:00000000
call.d SEQEVTATTIMEGE 0108 d:00000001
call.d SEQEVTATTIMEGT 0108 d:00000001
call.d SEQEVTATTIMEGE 0108 d:00000077
call.d SEQEVTATTIMEGT 0108 d:00000077
call.d SEQEVTATTIMEGE 0108 d:00000078
call.d SEQEVTATTIMEGT 0108 d:00000078
call.d SEQEVTATTIMEGE 0108 d:000000f0
call.d SEQEVTATTIMEGT 0108 d:000000f0
call.d SEQEVTATTIMEGE 0108 d:000001e0
call.d SEQEVTATTIMEGT 0108 d:000001e0
call.d SEQEVTATTIMEGE 0108 d:000001df
call.d SEQEVTATTIMEGT 0108 d:000001df
call.d SEQEVTATTIMEGE 0108 d:000003c0
call.d SEQEVTATTIMEGT 0108 d:000003c0
call.d SEQEVTATTIMEGE 0108 d:00010000
call.d SEQEVTATTIMEGT 0108 d:00010000
call.d SEQSTREAMDUR 0108
call.w SEQMEASHASEVTS 0108 0001
call.w SEQMEASHASEVTS 0108 0002
call.w SEQMEASHASEVTS 0108 0003
call.w SEQMEASHASEVTS 0108 0004
call.w SEQMEASHASEVTS 0108 0005
call.w SEQMEASHASEVTS 0108 0006
call.w SEQMEASHASEVTS 0108 0007
call.w SEQMEASHASEVTS 0108 0008
call.w SEQMEASHASEVTS 0108 0009
call.w SEQMEASHASEVTS 0108 000a
call.w SEQMEASHASEVTS 0108 000b
call.w SEQDESTROYSEQ 0001
echo -- ENG_SYSX
@load ENG_SYSX.WRK
echo stream 0100
call.d SEQSTREAMLEN 0100
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
call.d SEQEVTATTIMEGE 0100 d:00000000
call.d SEQEVTATTIMEGT 0100 d:00000000
call.d SEQEVTATTIMEGE 0100 d:00000001
call.d SEQEVTATTIMEGT 0100 d:00000001
call.d SEQEVTATTIMEGE 0100 d:00000077
call.d SEQEVTATTIMEGT 0100 d:00000077
call.d SEQEVTATTIMEGE 0100 d:00000078
call.d SEQEVTATTIMEGT 0100 d:00000078
call.d SEQEVTATTIMEGE 0100 d:000000f0
call.d SEQEVTATTIMEGT 0100 d:000000f0
call.d SEQEVTATTIMEGE 0100 d:000001e0
call.d SEQEVTATTIMEGT 0100 d:000001e0
call.d SEQEVTATTIMEGE 0100 d:000001df
call.d SEQEVTATTIMEGT 0100 d:000001df
call.d SEQEVTATTIMEGE 0100 d:000003c0
call.d SEQEVTATTIMEGT 0100 d:000003c0
call.d SEQEVTATTIMEGE 0100 d:00010000
call.d SEQEVTATTIMEGT 0100 d:00010000
call.d SEQSTREAMDUR 0100
call.w SEQMEASHASEVTS 0100 0001
call.w SEQMEASHASEVTS 0100 0002
call.w SEQMEASHASEVTS 0100 0003
call.w SEQMEASHASEVTS 0100 0004
call.w SEQMEASHASEVTS 0100 0005
call.w SEQMEASHASEVTS 0100 0006
call.w SEQMEASHASEVTS 0100 0007
call.w SEQMEASHASEVTS 0100 0008
call.w SEQMEASHASEVTS 0100 0009
call.w SEQMEASHASEVTS 0100 000a
call.w SEQMEASHASEVTS 0100 000b
call.w SEQDESTROYSEQ 0001
@close
