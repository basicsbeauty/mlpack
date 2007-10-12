def cores(name, strings):
  matrix = [x.split(",") for x in strings]
  dataset= matrix[0][0]
  sum = dict()
  sum[1] = []
  sum[2] = []
  sum[4] = []
  sum[8] = []
  for line in matrix:
    time = float(line[3])
    cores = float(line[5])
    sum[cores].append(time)
  val = {}
  for i in [1,2,4,8]:
    # median of 3
    val[i] = sorted(sum[i])[1]
  print "%s, %s, %.3f, %.2f, %.2f, %.2f || %.2f, %.2f" % (name, dataset, val[1], val[1]/val[2]/2, val[1]/val[4]/4, val[1]/val[8]/8, val[8], val[1]/val[8])

cores("tpclo", ["/tmp/garryb/lcdm1000k.txt,,18,16.902014,21.739924,8",
"/tmp/garryb/lcdm1000k.txt,,18,16.934927,21.776062,8",
"/tmp/garryb/lcdm1000k.txt,,18,16.978507,21.846815,8",
"/tmp/garryb/lcdm1000k.txt,,18,33.207969,38.048745,4",
"/tmp/garryb/lcdm1000k.txt,,18,33.213311,38.049898,4",
"/tmp/garryb/lcdm1000k.txt,,18,33.226755,38.061287,4",
"/tmp/garryb/lcdm1000k.txt,,18,66.664125,71.499118,2",
"/tmp/garryb/lcdm1000k.txt,,18,66.825097,71.658912,2",
"/tmp/garryb/lcdm1000k.txt,,18,66.828090,71.693306,2",
"/tmp/garryb/lcdm1000k.txt,,18,133.370977,138.209201,1",
"/tmp/garryb/lcdm1000k.txt,,18,133.456656,138.309714,1",
"/tmp/garryb/lcdm1000k.txt,,18,133.580445,138.416694,1",])

cores("tpchi", ["/tmp/garryb/lcdm1000k.txt,,90,141.420567,146.259684,8",
"/tmp/garryb/lcdm1000k.txt,,90,141.488462,146.373004,8",
"/tmp/garryb/lcdm1000k.txt,,90,142.004911,146.843606,8",
"/tmp/garryb/lcdm1000k.txt,,90,280.934574,285.769602,4",
"/tmp/garryb/lcdm1000k.txt,,90,281.028125,285.893865,4",
"/tmp/garryb/lcdm1000k.txt,,90,281.325056,286.160724,4",
"/tmp/garryb/lcdm1000k.txt,,90,562.944430,567.776923,2",
"/tmp/garryb/lcdm1000k.txt,,90,563.065030,567.931079,2",
"/tmp/garryb/lcdm1000k.txt,,90,568.195938,573.027907,2",
"/tmp/garryb/lcdm1000k.txt,,90,1122.680300,1127.518222,1",
"/tmp/garryb/lcdm1000k.txt,,90,1124.255670,1129.093656,1",
"/tmp/garryb/lcdm1000k.txt,,90,1124.508737,1129.365720,1"])

cores("kde", ["/tmp/garryb/a50k.txt,,0.05,11.831156,12.086961,8",
"/tmp/garryb/a50k.txt,,0.05,11.832288,12.088034,8",
"/tmp/garryb/a50k.txt,,0.05,11.848615,12.107277,8",
"/tmp/garryb/a50k.txt,,0.05,23.948390,24.206650,4",
"/tmp/garryb/a50k.txt,,0.05,23.971441,24.227474,4",
"/tmp/garryb/a50k.txt,,0.05,24.007455,24.262848,4",
"/tmp/garryb/a50k.txt,,0.05,47.771560,48.025613,2",
"/tmp/garryb/a50k.txt,,0.05,47.795281,48.049258,2",
"/tmp/garryb/a50k.txt,,0.05,48.473415,48.728647,2",
"/tmp/garryb/a50k.txt,,0.05,94.397373,94.653329,1",
"/tmp/garryb/a50k.txt,,0.05,94.609070,94.864434,1",
"/tmp/garryb/a50k.txt,,0.05,94.617997,94.873045,1",])

cores("allnn", ["/tmp/garryb/lcdm1000k.txt,,,0.283993,5.122605,8",
"/tmp/garryb/lcdm1000k.txt,,,0.287303,5.126089,8",
"/tmp/garryb/lcdm1000k.txt,,,0.288100,5.175230,8",
"/tmp/garryb/lcdm1000k.txt,,,0.545299,5.380927,4",
"/tmp/garryb/lcdm1000k.txt,,,0.545425,5.383879,4",
"/tmp/garryb/lcdm1000k.txt,,,0.546838,5.413778,4",
"/tmp/garryb/lcdm1000k.txt,,,1.106832,5.939838,2",
"/tmp/garryb/lcdm1000k.txt,,,1.107688,6.021290,2",
"/tmp/garryb/lcdm1000k.txt,,,1.109666,5.942334,2",
"/tmp/garryb/lcdm1000k.txt,,,2.206416,7.044109,1",
"/tmp/garryb/lcdm1000k.txt,,,2.208047,7.045871,1",
"/tmp/garryb/lcdm1000k.txt,,,2.218139,7.102688,1",])

cores("allnn", ["/tmp/garryb/lcdm100k.txt,,,0.032332,0.467600,8",
"/tmp/garryb/lcdm100k.txt,,,0.035167,0.471045,8",
"/tmp/garryb/lcdm100k.txt,,,0.035252,0.470815,8",
"/tmp/garryb/lcdm100k.txt,,,0.054950,0.489487,4",
"/tmp/garryb/lcdm100k.txt,,,0.055204,0.489996,4",
"/tmp/garryb/lcdm100k.txt,,,0.055488,0.489306,4",
"/tmp/garryb/lcdm100k.txt,,,0.103352,0.535224,2",
"/tmp/garryb/lcdm100k.txt,,,0.104927,0.537746,2",
"/tmp/garryb/lcdm100k.txt,,,0.104980,0.537747,2",
"/tmp/garryb/lcdm100k.txt,,,0.204686,0.639160,1",
"/tmp/garryb/lcdm100k.txt,,,0.204844,0.640767,1",
"/tmp/garryb/lcdm100k.txt,,,0.204971,0.637561,1",])

cores("allnn", ["/tmp/garryb/lcdm3000k.txt,,,0.869630,16.241188,8",
"/tmp/garryb/lcdm3000k.txt,,,0.870692,16.240841,8",
"/tmp/garryb/lcdm3000k.txt,,,0.883973,16.248429,8",
"/tmp/garryb/lcdm3000k.txt,,,1.701214,17.167242,4",
"/tmp/garryb/lcdm3000k.txt,,,1.703287,17.165781,4",
"/tmp/garryb/lcdm3000k.txt,,,1.735345,17.145664,4",
"/tmp/garryb/lcdm3000k.txt,,,3.467488,18.835986,2",
"/tmp/garryb/lcdm3000k.txt,,,3.467750,18.834184,2",
"/tmp/garryb/lcdm3000k.txt,,,3.500957,18.881582,2",
"/tmp/garryb/lcdm3000k.txt,,,6.915941,22.269450,1",
"/tmp/garryb/lcdm3000k.txt,,,6.941424,22.321224,1",
"/tmp/garryb/lcdm3000k.txt,,,6.944699,22.301685,1",])

cores("allnn", ["/tmp/garryb/timit100k.txt,,,3.953831,5.262230,8",
"/tmp/garryb/timit100k.txt,,,3.964844,5.260948,8",
"/tmp/garryb/timit100k.txt,,,3.990684,5.285639,8",
"/tmp/garryb/timit100k.txt,,,7.663731,8.958602,4",
"/tmp/garryb/timit100k.txt,,,7.835841,9.132875,4",
"/tmp/garryb/timit100k.txt,,,8.002063,9.295452,4",
"/tmp/garryb/timit100k.txt,,,15.383413,16.673510,2",
"/tmp/garryb/timit100k.txt,,,15.389958,16.680693,2",
"/tmp/garryb/timit100k.txt,,,15.600386,16.891483,2",
"/tmp/garryb/timit100k.txt,,,30.346106,31.640877,1",
"/tmp/garryb/timit100k.txt,,,30.348359,31.640579,1",
"/tmp/garryb/timit100k.txt,,,30.464669,31.758593,1",])
