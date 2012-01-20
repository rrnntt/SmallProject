/***************************************************************************
                          vjkg.h  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Roman N Tolchenov,A11,,roman@theory.phys.ucl.ac.uk
    email                : roman@phys.ucl.ac.uk.
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <math.h>
#include "voigt.h"


Voigt::Voigt(){
  xlast = 26.;
  old_dmp = -10.;
  p	= new double[26+1] = {0.,  1.064467, 1.081922, 1.099638, 1.117619,
         1.135864, 1.154377, 1.173157, 1.192206, 1.211524, 1.231108,
         1.250959, 1.271072, 1.291443, 1.312067, 1.332934, 1.354034,
         1.375351, 1.396866, 1.418555, 1.440383, 1.462309, 1.484277,
         1.506214, 1.528029, 1.549605, 1.570797};
  bgg	= new double[51+1] = {0.,        0.360674, 0.352999, 0.345341, 0.337700,
         0.330075, 0.322468, 0.314878, 0.307305, 0.299750, 0.292213, 0.284693,
         0.277192, 0.269708, 0.262243, 0.254797, 0.247369, 0.239960, 0.232570,
         0.225199, 0.217847, 0.210515, 0.203202, 0.195909, 0.188636, 0.181382,
         0.174148, 0.166935, 0.159742, 0.152569, 0.145416, 0.138284, 0.131172,
         0.124080, 0.117009, 0.109959, 0.102929, 0.095919, 0.088930, 0.081961,
         0.075013, 0.068084, 0.061176, 0.054288, 0.047420, 0.040573, 0.033747,
         0.026944, 0.020164, 0.013411, 0.006687, 0.000000};
vf[0][1]=0.000000;
vf[0][2]=0.000000;
vf[0][3]=0.000000;
vf[0][4]=0.000000;
vf[0][5]=0.000000;
vf[0][6]=0.000000;
vf[0][7]=0.000000;
vf[0][8]=0.000000;
vf[0][9]=0.000000;
vf[0][10]=0.000000;
vf[0][11]=0.000000;
vf[0][12]=0.000000;
vf[0][13]=0.000000;
vf[0][14]=0.000000;
vf[0][15]=0.000000;
vf[0][16]=0.000000;
vf[0][17]=0.000000;
vf[0][18]=0.000000;
vf[0][19]=0.000000;
vf[0][20]=0.000000;
vf[0][21]=0.000000;
vf[0][22]=0.000000;
vf[0][23]=0.000000;
vf[0][24]=0.000000;
vf[0][25]=0.000000;
vf[0][26]=0.000000;
vf[1][0]=0.000000;
vf[1][1]=1.000000;
vf[1][2]=1.000000;
vf[1][3]=1.000000;
vf[1][4]=1.000000;
vf[1][5]=1.000000;
vf[1][6]=1.000000;
vf[1][7]=1.000000;
vf[1][8]=1.000000;
vf[1][9]=1.000000;
vf[1][10]=1.000000;
vf[1][11]=1.000000;
vf[1][12]=1.000000;
vf[1][13]=1.000000;
vf[1][14]=1.000000;
vf[1][15]=1.000000;
vf[1][16]=1.000000;
vf[1][17]=1.000000;
vf[1][18]=1.000000;
vf[1][19]=1.000000;
vf[1][20]=1.000000;
vf[1][21]=1.000000;
vf[1][22]=1.000000;
vf[1][23]=1.000000;
vf[1][24]=1.000000;
vf[1][25]=1.000000;
vf[1][26]=1.000000;
vf[2][0]=0.000000;
vf[2][1]=0.993092;
vf[2][2]=0.993057;
vf[2][3]=0.993020;
vf[2][4]=0.992981;
vf[2][5]=0.992940;
vf[2][6]=0.992896;
vf[2][7]=0.992849;
vf[2][8]=0.992799;
vf[2][9]=0.992746;
vf[2][10]=0.992689;
vf[2][11]=0.992628;
vf[2][12]=0.992562;
vf[2][13]=0.992491;
vf[2][14]=0.992414;
vf[2][15]=0.992329;
vf[2][16]=0.992236;
vf[2][17]=0.992134;
vf[2][18]=0.992020;
vf[2][19]=0.991892;
vf[2][20]=0.991747;
vf[2][21]=0.991580;
vf[2][22]=0.991387;
vf[2][23]=0.991159;
vf[2][24]=0.990884;
vf[2][25]=0.990542;
vf[2][26]=0.990099;
vf[3][0]=0.000000;
vf[3][1]=0.972655;
vf[3][2]=0.972521;
vf[3][3]=0.972379;
vf[3][4]=0.972230;
vf[3][5]=0.972072;
vf[3][6]=0.971905;
vf[3][7]=0.971727;
vf[3][8]=0.971538;
vf[3][9]=0.971337;
vf[3][10]=0.971121;
vf[3][11]=0.970889;
vf[3][12]=0.970640;
vf[3][13]=0.970370;
vf[3][14]=0.970077;
vf[3][15]=0.969758;
vf[3][16]=0.969407;
vf[3][17]=0.969021;
vf[3][18]=0.968592;
vf[3][19]=0.968111;
vf[3][20]=0.967568;
vf[3][21]=0.966948;
vf[3][22]=0.966231;
vf[3][23]=0.965387;
vf[3][24]=0.964375;
vf[3][25]=0.963129;
vf[3][26]=0.961539;
vf[4][0]=0.000000;
vf[4][1]=0.939523;
vf[4][2]=0.939244;
vf[4][3]=0.938951;
vf[4][4]=0.938642;
vf[4][5]=0.938316;
vf[4][6]=0.937970;
vf[4][7]=0.937603;
vf[4][8]=0.937214;
vf[4][9]=0.936798;
vf[4][10]=0.936354;
vf[4][11]=0.935878;
vf[4][12]=0.935365;
vf[4][13]=0.934813;
vf[4][14]=0.934213;
vf[4][15]=0.933561;
vf[4][16]=0.932848;
vf[4][17]=0.932062;
vf[4][18]=0.931193;
vf[4][19]=0.930223;
vf[4][20]=0.929131;
vf[4][21]=0.927890;
vf[4][22]=0.926463;
vf[4][23]=0.924798;
vf[4][24]=0.922821;
vf[4][25]=0.920423;
vf[4][26]=0.917431;
vf[5][0]=0.000000;
vf[5][1]=0.895025;
vf[5][2]=0.894586;
vf[5][3]=0.894124;
vf[5][4]=0.893637;
vf[5][5]=0.893124;
vf[5][6]=0.892581;
vf[5][7]=0.892006;
vf[5][8]=0.891395;
vf[5][9]=0.890745;
vf[5][10]=0.890051;
vf[5][11]=0.889309;
vf[5][12]=0.888512;
vf[5][13]=0.887654;
vf[5][14]=0.886727;
vf[5][15]=0.885719;
vf[5][16]=0.884621;
vf[5][17]=0.883416;
vf[5][18]=0.882088;
vf[5][19]=0.880612;
vf[5][20]=0.878961;
vf[5][21]=0.877097;
vf[5][22]=0.874971;
vf[5][23]=0.872515;
vf[5][24]=0.869640;
vf[5][25]=0.866219;
vf[5][26]=0.862069;
vf[6][0]=0.000000;
vf[6][1]=0.840896;
vf[6][2]=0.840314;
vf[6][3]=0.839702;
vf[6][4]=0.839058;
vf[6][5]=0.838380;
vf[6][6]=0.837664;
vf[6][7]=0.836906;
vf[6][8]=0.836103;
vf[6][9]=0.835251;
vf[6][10]=0.834343;
vf[6][11]=0.833373;
vf[6][12]=0.832335;
vf[6][13]=0.831220;
vf[6][14]=0.830019;
vf[6][15]=0.828719;
vf[6][16]=0.827306;
vf[6][17]=0.825764;
vf[6][18]=0.824072;
vf[6][19]=0.822204;
vf[6][20]=0.820127;
vf[6][21]=0.817802;
vf[6][22]=0.815176;
vf[6][23]=0.812182;
vf[6][24]=0.808733;
vf[6][25]=0.804718;
vf[6][26]=0.800000;
vf[7][0]=0.000000;
vf[7][1]=0.779165;
vf[7][2]=0.778491;
vf[7][3]=0.777785;
vf[7][4]=0.777044;
vf[7][5]=0.776264;
vf[7][6]=0.775442;
vf[7][7]=0.774574;
vf[7][8]=0.773657;
vf[7][9]=0.772684;
vf[7][10]=0.771652;
vf[7][11]=0.770552;
vf[7][12]=0.769378;
vf[7][13]=0.768122;
vf[7][14]=0.766773;
vf[7][15]=0.765319;
vf[7][16]=0.763746;
vf[7][17]=0.762039;
vf[7][18]=0.760176;
vf[7][19]=0.758133;
vf[7][20]=0.755881;
vf[7][21]=0.753383;
vf[7][22]=0.750594;
vf[7][23]=0.747459;
vf[7][24]=0.743913;
vf[7][25]=0.739882;
vf[7][26]=0.735295;
vf[8][0]=0.000000;
vf[8][1]=0.712025;
vf[8][2]=0.711346;
vf[8][3]=0.710635;
vf[8][4]=0.709890;
vf[8][5]=0.709107;
vf[8][6]=0.708285;
vf[8][7]=0.707418;
vf[8][8]=0.706504;
vf[8][9]=0.705538;
vf[8][10]=0.704515;
vf[8][11]=0.703430;
vf[8][12]=0.702275;
vf[8][13]=0.701043;
vf[8][14]=0.699726;
vf[8][15]=0.698314;
vf[8][16]=0.696794;
vf[8][17]=0.695153;
vf[8][18]=0.693375;
vf[8][19]=0.691441;
vf[8][20]=0.689327;
vf[8][21]=0.687007;
vf[8][22]=0.684450;
vf[8][23]=0.681619;
vf[8][24]=0.678477;
vf[8][25]=0.674991;
vf[8][26]=0.671141;
vf[9][0]=0.000000;
vf[9][1]=0.641713;
vf[9][2]=0.641137;
vf[9][3]=0.640535;
vf[9][4]=0.639905;
vf[9][5]=0.639246;
vf[9][6]=0.638554;
vf[9][7]=0.637827;
vf[9][8]=0.637063;
vf[9][9]=0.636258;
vf[9][10]=0.635408;
vf[9][11]=0.634509;
vf[9][12]=0.633556;
vf[9][13]=0.632545;
vf[9][14]=0.631468;
vf[9][15]=0.630320;
vf[9][16]=0.629091;
vf[9][17]=0.627773;
vf[9][18]=0.626355;
vf[9][19]=0.624826;
vf[9][20]=0.623170;
vf[9][21]=0.621374;
vf[9][22]=0.619419;
vf[9][23]=0.617290;
vf[9][24]=0.614972;
vf[9][25]=0.612457;
vf[9][26]=0.609756;
vf[10][0]=0.000000;
vf[10][1]=0.570382;
vf[10][2]=0.570032;
vf[10][3]=0.569666;
vf[10][4]=0.569285;
vf[10][5]=0.568887;
vf[10][6]=0.568470;
vf[10][7]=0.568034;
vf[10][8]=0.567577;
vf[10][9]=0.567096;
vf[10][10]=0.566591;
vf[10][11]=0.566059;
vf[10][12]=0.565497;
vf[10][13]=0.564904;
vf[10][14]=0.564275;
vf[10][15]=0.563608;
vf[10][16]=0.562899;
vf[10][17]=0.562144;
vf[10][18]=0.561338;
vf[10][19]=0.560476;
vf[10][20]=0.559553;
vf[10][21]=0.558562;
vf[10][22]=0.557499;
vf[10][23]=0.556358;
vf[10][24]=0.555139;
vf[10][25]=0.553845;
vf[10][26]=0.552486;
vf[11][0]=0.000000;
vf[11][1]=0.500000;
vf[11][2]=0.500000;
vf[11][3]=0.500000;
vf[11][4]=0.500000;
vf[11][5]=0.500000;
vf[11][6]=0.500000;
vf[11][7]=0.500000;
vf[11][8]=0.500000;
vf[11][9]=0.500000;
vf[11][10]=0.500000;
vf[11][11]=0.500000;
vf[11][12]=0.500000;
vf[11][13]=0.500000;
vf[11][14]=0.500000;
vf[11][15]=0.500000;
vf[11][16]=0.500000;
vf[11][17]=0.500000;
vf[11][18]=0.500000;
vf[11][19]=0.500000;
vf[11][20]=0.500000;
vf[11][21]=0.500000;
vf[11][22]=0.500000;
vf[11][23]=0.500000;
vf[11][24]=0.500000;
vf[11][25]=0.500000;
vf[11][26]=0.500000;
vf[12][0]=0.000000;
vf[12][1]=0.432269;
vf[12][2]=0.432733;
vf[12][3]=0.433215;
vf[12][4]=0.433716;
vf[12][5]=0.434236;
vf[12][6]=0.434776;
vf[12][7]=0.435338;
vf[12][8]=0.435924;
vf[12][9]=0.436534;
vf[12][10]=0.437171;
vf[12][11]=0.437836;
vf[12][12]=0.438531;
vf[12][13]=0.439258;
vf[12][14]=0.440020;
vf[12][15]=0.440819;
vf[12][16]=0.441656;
vf[12][17]=0.442535;
vf[12][18]=0.443459;
vf[12][19]=0.444429;
vf[12][20]=0.445447;
vf[12][21]=0.446514;
vf[12][22]=0.447631;
vf[12][23]=0.448795;
vf[12][24]=0.450000;
vf[12][25]=0.451236;
vf[12][26]=0.452489;
vf[13][0]=0.000000;
vf[13][1]=0.368567;
vf[13][2]=0.369590;
vf[13][3]=0.370649;
vf[13][4]=0.371744;
vf[13][5]=0.372878;
vf[13][6]=0.374053;
vf[13][7]=0.375272;
vf[13][8]=0.376536;
vf[13][9]=0.377849;
vf[13][10]=0.379212;
vf[13][11]=0.380630;
vf[13][12]=0.382105;
vf[13][13]=0.383639;
vf[13][14]=0.385237;
vf[13][15]=0.386901;
vf[13][16]=0.388636;
vf[13][17]=0.390443;
vf[13][18]=0.392325;
vf[13][19]=0.394285;
vf[13][20]=0.396322;
vf[13][21]=0.398436;
vf[13][22]=0.400622;
vf[13][23]=0.402872;
vf[13][24]=0.405173;
vf[13][25]=0.407503;
vf[13][26]=0.409836;
vf[14][0]=0.000000;
vf[14][1]=0.309927;
vf[14][2]=0.311573;
vf[14][3]=0.313271;
vf[14][4]=0.315023;
vf[14][5]=0.316832;
vf[14][6]=0.318699;
vf[14][7]=0.320628;
vf[14][8]=0.322622;
vf[14][9]=0.324684;
vf[14][10]=0.326816;
vf[14][11]=0.329022;
vf[14][12]=0.331306;
vf[14][13]=0.333669;
vf[14][14]=0.336116;
vf[14][15]=0.338650;
vf[14][16]=0.341271;
vf[14][17]=0.343984;
vf[14][18]=0.346787;
vf[14][19]=0.349681;
vf[14][20]=0.352663;
vf[14][21]=0.355727;
vf[14][22]=0.358864;
vf[14][23]=0.362058;
vf[14][24]=0.365290;
vf[14][25]=0.368530;
vf[14][26]=0.371747;
vf[15][0]=0.000000;
vf[15][1]=0.257028;
vf[15][2]=0.259330;
vf[15][3]=0.261696;
vf[15][4]=0.264129;
vf[15][5]=0.266633;
vf[15][6]=0.269209;
vf[15][7]=0.271861;
vf[15][8]=0.274590;
vf[15][9]=0.277401;
vf[15][10]=0.280295;
vf[15][11]=0.283276;
vf[15][12]=0.286345;
vf[15][13]=0.289506;
vf[15][14]=0.292759;
vf[15][15]=0.296107;
vf[15][16]=0.299549;
vf[15][17]=0.303086;
vf[15][18]=0.306715;
vf[15][19]=0.310433;
vf[15][20]=0.314232;
vf[15][21]=0.318103;
vf[15][22]=0.322030;
vf[15][23]=0.325996;
vf[15][24]=0.329973;
vf[15][25]=0.333931;
vf[15][26]=0.337838;
vf[16][0]=0.000000;
vf[16][1]=0.210224;
vf[16][2]=0.213177;
vf[16][3]=0.216204;
vf[16][4]=0.219307;
vf[16][5]=0.222488;
vf[16][6]=0.225750;
vf[16][7]=0.229095;
vf[16][8]=0.232525;
vf[16][9]=0.236042;
vf[16][10]=0.239647;
vf[16][11]=0.243344;
vf[16][12]=0.247131;
vf[16][13]=0.251011;
vf[16][14]=0.254984;
vf[16][15]=0.259048;
vf[16][16]=0.263202;
vf[16][17]=0.267442;
vf[16][18]=0.271764;
vf[16][19]=0.276161;
vf[16][20]=0.280622;
vf[16][21]=0.285133;
vf[16][22]=0.289679;
vf[16][23]=0.294236;
vf[16][24]=0.298778;
vf[16][25]=0.303274;
vf[16][26]=0.307692;
vf[17][0]=0.000000;
vf[17][1]=0.169576;
vf[17][2]=0.173144;
vf[17][3]=0.176790;
vf[17][4]=0.180516;
vf[17][5]=0.184324;
vf[17][6]=0.188214;
vf[17][7]=0.192188;
vf[17][8]=0.196247;
vf[17][9]=0.200392;
vf[17][10]=0.204624;
vf[17][11]=0.208943;
vf[17][12]=0.213348;
vf[17][13]=0.217838;
vf[17][14]=0.222411;
vf[17][15]=0.227065;
vf[17][16]=0.231796;
vf[17][17]=0.236597;
vf[17][18]=0.241461;
vf[17][19]=0.246380;
vf[17][20]=0.251340;
vf[17][21]=0.256327;
vf[17][22]=0.261322;
vf[17][23]=0.266304;
vf[17][24]=0.271246;
vf[17][25]=0.276121;
vf[17][26]=0.280899;
vf[18][0]=0.000000;
vf[18][1]=0.134904;
vf[18][2]=0.139023;
vf[18][3]=0.143221;
vf[18][4]=0.147496;
vf[18][5]=0.151849;
vf[18][6]=0.156282;
vf[18][7]=0.160794;
vf[18][8]=0.165386;
vf[18][9]=0.170056;
vf[18][10]=0.174805;
vf[18][11]=0.179630;
vf[18][12]=0.184531;
vf[18][13]=0.189503;
vf[18][14]=0.194543;
vf[18][15]=0.199648;
vf[18][16]=0.204811;
vf[18][17]=0.210024;
vf[18][18]=0.215280;
vf[18][19]=0.220567;
vf[18][20]=0.225872;
vf[18][21]=0.231181;
vf[18][22]=0.236476;
vf[18][23]=0.241736;
vf[18][24]=0.246939;
vf[18][25]=0.252058;
vf[18][26]=0.257070;
vf[19][0]=0.000000;
vf[19][1]=0.105843;
vf[19][2]=0.110429;
vf[19][3]=0.115087;
vf[19][4]=0.119816;
vf[19][5]=0.124617;
vf[19][6]=0.129489;
vf[19][7]=0.134431;
vf[19][8]=0.139442;
vf[19][9]=0.144520;
vf[19][10]=0.149663;
vf[19][11]=0.154869;
vf[19][12]=0.160135;
vf[19][13]=0.165455;
vf[19][14]=0.170827;
vf[19][15]=0.176243;
vf[19][16]=0.181698;
vf[19][17]=0.187182;
vf[19][18]=0.192688;
vf[19][19]=0.198205;
vf[19][20]=0.203719;
vf[19][21]=0.209218;
vf[19][22]=0.214686;
vf[19][23]=0.220103;
vf[19][24]=0.225451;
vf[19][25]=0.230707;
vf[19][26]=0.235849;
vf[20][0]=0.000000;
vf[20][1]=0.081900;
vf[20][2]=0.086852;
vf[20][3]=0.091868;
vf[20][4]=0.096945;
vf[20][5]=0.102084;
vf[20][6]=0.107281;
vf[20][7]=0.112537;
vf[20][8]=0.117848;
vf[20][9]=0.123211;
vf[20][10]=0.128625;
vf[20][11]=0.134086;
vf[20][12]=0.139589;
vf[20][13]=0.145129;
vf[20][14]=0.150702;
vf[20][15]=0.156302;
vf[20][16]=0.161921;
vf[20][17]=0.167552;
vf[20][18]=0.173186;
vf[20][19]=0.178813;
vf[20][20]=0.184423;
vf[20][21]=0.190003;
vf[20][22]=0.195540;
vf[20][23]=0.201018;
vf[20][24]=0.206420;
vf[20][25]=0.211727;
vf[20][26]=0.216920;
vf[21][0]=0.000000;
vf[21][1]=0.062500;
vf[21][2]=0.067714;
vf[21][3]=0.072979;
vf[21][4]=0.078294;
vf[21][5]=0.083658;
vf[21][6]=0.089067;
vf[21][7]=0.094521;
vf[21][8]=0.100015;
vf[21][9]=0.105547;
vf[21][10]=0.111114;
vf[21][11]=0.116711;
vf[21][12]=0.122334;
vf[21][13]=0.127978;
vf[21][14]=0.133639;
vf[21][15]=0.139310;
vf[21][16]=0.144984;
vf[21][17]=0.150656;
vf[21][18]=0.156317;
vf[21][19]=0.161959;
vf[21][20]=0.167573;
vf[21][21]=0.173149;
vf[21][22]=0.178676;
vf[21][23]=0.184139;
vf[21][24]=0.189526;
vf[21][25]=0.194819;
vf[21][26]=0.200000;
vf[22][0]=0.000000;
vf[22][1]=0.047039;
vf[22][2]=0.052408;
vf[22][3]=0.057817;
vf[22][4]=0.063263;
vf[22][5]=0.068744;
vf[22][6]=0.074258;
vf[22][7]=0.079802;
vf[22][8]=0.085372;
vf[22][9]=0.090966;
vf[22][10]=0.096580;
vf[22][11]=0.102210;
vf[22][12]=0.107852;
vf[22][13]=0.113502;
vf[22][14]=0.119154;
vf[22][15]=0.124804;
vf[22][16]=0.130446;
vf[22][17]=0.136075;
vf[22][18]=0.141684;
vf[22][19]=0.147267;
vf[22][20]=0.152816;
vf[22][21]=0.158323;
vf[22][22]=0.163779;
vf[22][23]=0.169172;
vf[22][24]=0.174491;
vf[22][25]=0.179720;
vf[22][26]=0.184843;
vf[23][0]=0.000000;
vf[23][1]=0.034915;
vf[23][2]=0.040342;
vf[23][3]=0.045796;
vf[23][4]=0.051274;
vf[23][5]=0.056776;
vf[23][6]=0.062298;
vf[23][7]=0.067837;
vf[23][8]=0.073390;
vf[23][9]=0.078954;
vf[23][10]=0.084527;
vf[23][11]=0.090103;
vf[23][12]=0.095681;
vf[23][13]=0.101256;
vf[23][14]=0.106824;
vf[23][15]=0.112382;
vf[23][16]=0.117924;
vf[23][17]=0.123446;
vf[23][18]=0.128944;
vf[23][19]=0.134413;
vf[23][20]=0.139846;
vf[23][21]=0.145237;
vf[23][22]=0.150579;
vf[23][23]=0.155861;
vf[23][24]=0.161074;
vf[23][25]=0.166202;
vf[23][26]=0.171233;
vf[24][0]=0.000000;
vf[24][1]=0.025559;
vf[24][2]=0.030955;
vf[24][3]=0.036368;
vf[24][4]=0.041794;
vf[24][5]=0.047233;
vf[24][6]=0.052680;
vf[24][7]=0.058135;
vf[24][8]=0.063594;
vf[24][9]=0.069055;
vf[24][10]=0.074515;
vf[24][11]=0.079971;
vf[24][12]=0.085420;
vf[24][13]=0.090859;
vf[24][14]=0.096286;
vf[24][15]=0.101698;
vf[24][16]=0.107091;
vf[24][17]=0.112461;
vf[24][18]=0.117807;
vf[24][19]=0.123123;
vf[24][20]=0.128405;
vf[24][21]=0.133647;
vf[24][22]=0.138844;
vf[24][23]=0.143987;
vf[24][24]=0.149066;
vf[24][25]=0.154069;
vf[24][26]=0.158983;
vf[25][0]=0.000000;
vf[25][1]=0.018453;
vf[25][2]=0.023744;
vf[25][3]=0.029043;
vf[25][4]=0.034347;
vf[25][5]=0.039654;
vf[25][6]=0.044963;
vf[25][7]=0.050271;
vf[25][8]=0.055576;
vf[25][9]=0.060877;
vf[25][10]=0.066170;
vf[25][11]=0.071455;
vf[25][12]=0.076729;
vf[25][13]=0.081990;
vf[25][14]=0.087235;
vf[25][15]=0.092464;
vf[25][16]=0.097674;
vf[25][17]=0.102863;
vf[25][18]=0.108027;
vf[25][19]=0.113165;
vf[25][20]=0.118273;
vf[25][21]=0.123346;
vf[25][22]=0.128378;
vf[25][23]=0.133363;
vf[25][24]=0.138291;
vf[25][25]=0.143150;
vf[25][26]=0.147929;
vf[26][0]=0.000000;
vf[26][1]=0.013139;
vf[26][2]=0.018267;
vf[26][3]=0.023395;
vf[26][4]=0.028522;
vf[26][5]=0.033646;
vf[26][6]=0.038767;
vf[26][7]=0.043882;
vf[26][8]=0.048990;
vf[26][9]=0.054090;
vf[26][10]=0.059180;
vf[26][11]=0.064259;
vf[26][12]=0.069326;
vf[26][13]=0.074379;
vf[26][14]=0.079418;
vf[26][15]=0.084441;
vf[26][16]=0.089448;
vf[26][17]=0.094435;
vf[26][18]=0.099403;
vf[26][19]=0.104347;
vf[26][20]=0.109267;
vf[26][21]=0.114157;
vf[26][22]=0.119013;
vf[26][23]=0.123828;
vf[26][24]=0.128593;
vf[26][25]=0.133298;
vf[26][26]=0.137931;
vf[27][0]=0.000000;
vf[27][1]=0.009227;
vf[27][2]=0.014146;
vf[27][3]=0.019062;
vf[27][4]=0.023973;
vf[27][5]=0.028878;
vf[27][6]=0.033777;
vf[27][7]=0.038668;
vf[27][8]=0.043550;
vf[27][9]=0.048423;
vf[27][10]=0.053286;
vf[27][11]=0.058138;
vf[27][12]=0.062980;
vf[27][13]=0.067810;
vf[27][14]=0.072627;
vf[27][15]=0.077432;
vf[27][16]=0.082224;
vf[27][17]=0.087001;
vf[27][18]=0.091763;
vf[27][19]=0.096507;
vf[27][20]=0.101232;
vf[27][21]=0.105933;
vf[27][22]=0.110606;
vf[27][23]=0.115245;
vf[27][24]=0.119841;
vf[27][25]=0.124386;
vf[27][26]=0.128866;
vf[28][0]=0.000000;
vf[28][1]=0.006390;
vf[28][2]=0.011072;
vf[28][3]=0.015748;
vf[28][4]=0.020417;
vf[28][5]=0.025080;
vf[28][6]=0.029736;
vf[28][7]=0.034384;
vf[28][8]=0.039024;
vf[28][9]=0.043656;
vf[28][10]=0.048280;
vf[28][11]=0.052896;
vf[28][12]=0.057503;
vf[28][13]=0.062102;
vf[28][14]=0.066693;
vf[28][15]=0.071276;
vf[28][16]=0.075849;
vf[28][17]=0.080413;
vf[28][18]=0.084967;
vf[28][19]=0.089509;
vf[28][20]=0.094037;
vf[28][21]=0.098547;
vf[28][22]=0.103035;
vf[28][23]=0.107496;
vf[28][24]=0.111922;
vf[28][25]=0.116303;
vf[28][26]=0.120627;
vf[29][0]=0.000000;
vf[29][1]=0.004364;
vf[29][2]=0.008791;
vf[29][3]=0.013211;
vf[29][4]=0.017626;
vf[29][5]=0.022035;
vf[29][6]=0.026438;
vf[29][7]=0.030835;
vf[29][8]=0.035227;
vf[29][9]=0.039613;
vf[29][10]=0.043995;
vf[29][11]=0.048372;
vf[29][12]=0.052745;
vf[29][13]=0.057113;
vf[29][14]=0.061478;
vf[29][15]=0.065839;
vf[29][16]=0.070196;
vf[29][17]=0.074549;
vf[29][18]=0.078897;
vf[29][19]=0.083238;
vf[29][20]=0.087571;
vf[29][21]=0.091892;
vf[29][22]=0.096198;
vf[29][23]=0.100481;
vf[29][24]=0.104737;
vf[29][25]=0.108954;
vf[29][26]=0.113122;
vf[30][0]=0.000000;
vf[30][1]=0.002940;
vf[30][2]=0.007104;
vf[30][3]=0.011263;
vf[30][4]=0.015419;
vf[30][5]=0.019571;
vf[30][6]=0.023721;
vf[30][7]=0.027868;
vf[30][8]=0.032012;
vf[30][9]=0.036156;
vf[30][10]=0.040298;
vf[30][11]=0.044440;
vf[30][12]=0.048582;
vf[30][13]=0.052725;
vf[30][14]=0.056869;
vf[30][15]=0.061014;
vf[30][16]=0.065160;
vf[30][17]=0.069307;
vf[30][18]=0.073454;
vf[30][19]=0.077600;
vf[30][20]=0.081742;
vf[30][21]=0.085878;
vf[30][22]=0.090004;
vf[30][23]=0.094114;
vf[30][24]=0.098201;
vf[30][25]=0.102256;
vf[30][26]=0.106270;
vf[31][0]=0.000000;
vf[31][1]=0.001953;
vf[31][2]=0.005855;
vf[31][3]=0.009756;
vf[31][4]=0.013657;
vf[31][5]=0.017557;
vf[31][6]=0.021458;
vf[31][7]=0.025360;
vf[31][8]=0.029265;
vf[31][9]=0.033172;
vf[31][10]=0.037083;
vf[31][11]=0.040998;
vf[31][12]=0.044918;
vf[31][13]=0.048843;
vf[31][14]=0.052774;
vf[31][15]=0.056712;
vf[31][16]=0.060654;
vf[31][17]=0.064603;
vf[31][18]=0.068556;
vf[31][19]=0.072512;
vf[31][20]=0.076470;
vf[31][21]=0.080426;
vf[31][22]=0.084377;
vf[31][23]=0.088318;
vf[31][24]=0.092241;
vf[31][25]=0.096139;
vf[31][26]=0.100000;
vf[32][0]=0.000000;
vf[32][1]=0.001280;
vf[32][2]=0.004928;
vf[32][3]=0.008578;
vf[32][4]=0.012232;
vf[32][5]=0.015890;
vf[32][6]=0.019552;
vf[32][7]=0.023220;
vf[32][8]=0.026895;
vf[32][9]=0.030577;
vf[32][10]=0.034267;
vf[32][11]=0.037965;
vf[32][12]=0.041673;
vf[32][13]=0.045391;
vf[32][14]=0.049119;
vf[32][15]=0.052858;
vf[32][16]=0.056606;
vf[32][17]=0.060365;
vf[32][18]=0.064132;
vf[32][19]=0.067907;
vf[32][20]=0.071687;
vf[32][21]=0.075471;
vf[32][22]=0.079253;
vf[32][23]=0.083030;
vf[32][24]=0.086794;
vf[32][25]=0.090537;
vf[32][26]=0.094251;
vf[33][0]=0.000000;
vf[33][1]=0.000827;
vf[33][2]=0.004232;
vf[33][3]=0.007645;
vf[33][4]=0.011064;
vf[33][5]=0.014491;
vf[33][6]=0.017928;
vf[33][7]=0.021375;
vf[33][8]=0.024832;
vf[33][9]=0.028301;
vf[33][10]=0.031782;
vf[33][11]=0.035276;
vf[33][12]=0.038784;
vf[33][13]=0.042306;
vf[33][14]=0.045841;
vf[33][15]=0.049391;
vf[33][16]=0.052956;
vf[33][17]=0.056533;
vf[33][18]=0.060123;
vf[33][19]=0.063725;
vf[33][20]=0.067336;
vf[33][21]=0.070953;
vf[33][22]=0.074574;
vf[33][23]=0.078193;
vf[33][24]=0.081804;
vf[33][25]=0.085398;
vf[33][26]=0.088968;
vf[34][0]=0.000000;
vf[34][1]=0.000527;
vf[34][2]=0.003705;
vf[34][3]=0.006893;
vf[34][4]=0.010092;
vf[34][5]=0.013304;
vf[34][6]=0.016529;
vf[34][7]=0.019768;
vf[34][8]=0.023022;
vf[34][9]=0.026291;
vf[34][10]=0.029576;
vf[34][11]=0.032879;
vf[34][12]=0.036198;
vf[34][13]=0.039535;
vf[34][14]=0.042889;
vf[34][15]=0.046262;
vf[34][16]=0.049651;
vf[34][17]=0.053058;
vf[34][18]=0.056480;
vf[34][19]=0.059917;
vf[34][20]=0.063366;
vf[34][21]=0.066825;
vf[34][22]=0.070291;
vf[34][23]=0.073758;
vf[34][24]=0.077222;
vf[34][25]=0.080674;
vf[34][26]=0.084104;
vf[35][0]=0.000000;
vf[35][1]=0.000331;
vf[35][2]=0.003297;
vf[35][3]=0.006276;
vf[35][4]=0.009272;
vf[35][5]=0.012283;
vf[35][6]=0.015311;
vf[35][7]=0.018357;
vf[35][8]=0.021421;
vf[35][9]=0.024504;
vf[35][10]=0.027607;
vf[35][11]=0.030730;
vf[35][12]=0.033873;
vf[35][13]=0.037036;
vf[35][14]=0.040221;
vf[35][15]=0.043426;
vf[35][16]=0.046651;
vf[35][17]=0.049895;
vf[35][18]=0.053158;
vf[35][19]=0.056439;
vf[35][20]=0.059735;
vf[35][21]=0.063043;
vf[35][22]=0.066361;
vf[35][23]=0.069684;
vf[35][24]=0.073006;
vf[35][25]=0.076321;
vf[35][26]=0.079618;
vf[36][0]=0.000000;
vf[36][1]=0.000205;
vf[36][2]=0.002975;
vf[36][3]=0.005762;
vf[36][4]=0.008569;
vf[36][5]=0.011395;
vf[36][6]=0.014241;
vf[36][7]=0.017107;
vf[36][8]=0.019996;
vf[36][9]=0.022906;
vf[36][10]=0.025839;
vf[36][11]=0.028794;
vf[36][12]=0.031773;
vf[36][13]=0.034775;
vf[36][14]=0.037799;
vf[36][15]=0.040847;
vf[36][16]=0.043917;
vf[36][17]=0.047009;
vf[36][18]=0.050122;
vf[36][19]=0.053255;
vf[36][20]=0.056405;
vf[36][21]=0.059570;
vf[36][22]=0.062748;
vf[36][23]=0.065933;
vf[36][24]=0.069120;
vf[36][25]=0.072303;
vf[36][26]=0.075472;
vf[37][0]=0.000000;
vf[37][1]=0.000126;
vf[37][2]=0.002715;
vf[37][3]=0.005326;
vf[37][4]=0.007959;
vf[37][5]=0.010614;
vf[37][6]=0.013292;
vf[37][7]=0.015994;
vf[37][8]=0.018719;
vf[37][9]=0.021470;
vf[37][10]=0.024244;
vf[37][11]=0.027044;
vf[37][12]=0.029869;
vf[37][13]=0.032720;
vf[37][14]=0.035595;
vf[37][15]=0.038495;
vf[37][16]=0.041420;
vf[37][17]=0.044368;
vf[37][18]=0.047340;
vf[37][19]=0.050332;
vf[37][20]=0.053345;
vf[37][21]=0.056374;
vf[37][22]=0.059418;
vf[37][23]=0.062472;
vf[37][24]=0.065530;
vf[37][25]=0.068587;
vf[37][26]=0.071633;
vf[38][0]=0.000000;
vf[38][1]=0.000076;
vf[38][2]=0.002501;
vf[38][3]=0.004950;
vf[38][4]=0.007424;
vf[38][5]=0.009922;
vf[38][6]=0.012445;
vf[38][7]=0.014995;
vf[38][8]=0.017570;
vf[38][9]=0.020172;
vf[38][10]=0.022800;
vf[38][11]=0.025455;
vf[38][12]=0.028137;
vf[38][13]=0.030846;
vf[38][14]=0.033582;
vf[38][15]=0.036344;
vf[38][16]=0.039132;
vf[38][17]=0.041945;
vf[38][18]=0.044783;
vf[38][19]=0.047643;
vf[38][20]=0.050525;
vf[38][21]=0.053426;
vf[38][22]=0.056343;
vf[38][23]=0.059272;
vf[38][24]=0.062208;
vf[38][25]=0.065144;
vf[38][26]=0.068074;
vf[39][0]=0.000000;
vf[39][1]=0.000045;
vf[39][2]=0.002321;
vf[39][3]=0.004622;
vf[39][4]=0.006949;
vf[39][5]=0.009304;
vf[39][6]=0.011685;
vf[39][7]=0.014094;
vf[39][8]=0.016530;
vf[39][9]=0.018994;
vf[39][10]=0.021487;
vf[39][11]=0.024007;
vf[39][12]=0.026556;
vf[39][13]=0.029133;
vf[39][14]=0.031738;
vf[39][15]=0.034371;
vf[39][16]=0.037030;
vf[39][17]=0.039717;
vf[39][18]=0.042428;
vf[39][19]=0.045164;
vf[39][20]=0.047923;
vf[39][21]=0.050702;
vf[39][22]=0.053499;
vf[39][23]=0.056309;
vf[39][24]=0.059128;
vf[39][25]=0.061950;
vf[39][26]=0.064767;
vf[40][0]=0.000000;
vf[40][1]=0.000026;
vf[40][2]=0.002165;
vf[40][3]=0.004331;
vf[40][4]=0.006525;
vf[40][5]=0.008747;
vf[40][6]=0.010998;
vf[40][7]=0.013277;
vf[40][8]=0.015585;
vf[40][9]=0.017922;
vf[40][10]=0.020288;
vf[40][11]=0.022684;
vf[40][12]=0.025108;
vf[40][13]=0.027562;
vf[40][14]=0.030045;
vf[40][15]=0.032556;
vf[40][16]=0.035095;
vf[40][17]=0.037662;
vf[40][18]=0.040255;
vf[40][19]=0.042874;
vf[40][20]=0.045516;
vf[40][21]=0.048179;
vf[40][22]=0.050862;
vf[40][23]=0.053560;
vf[40][24]=0.056268;
vf[40][25]=0.058980;
vf[40][26]=0.061691;
vf[41][0]=0.000000;
vf[41][1]=0.000015;
vf[41][2]=0.002029;
vf[41][3]=0.004072;
vf[41][4]=0.006143;
vf[41][5]=0.008244;
vf[41][6]=0.010374;
vf[41][7]=0.012533;
vf[41][8]=0.014723;
vf[41][9]=0.016942;
vf[41][10]=0.019191;
vf[41][11]=0.021470;
vf[41][12]=0.023779;
vf[41][13]=0.026118;
vf[41][14]=0.028486;
vf[41][15]=0.030883;
vf[41][16]=0.033310;
vf[41][17]=0.035764;
vf[41][18]=0.038245;
vf[41][19]=0.040753;
vf[41][20]=0.043285;
vf[41][21]=0.045839;
vf[41][22]=0.048414;
vf[41][23]=0.051004;
vf[41][24]=0.053607;
vf[41][25]=0.056216;
vf[41][26]=0.058824;
vf[42][0]=0.000000;
vf[42][1]=0.000009;
vf[42][2]=0.001909;
vf[42][3]=0.003838;
vf[42][4]=0.005797;
vf[42][5]=0.007786;
vf[42][6]=0.009805;
vf[42][7]=0.011854;
vf[42][8]=0.013934;
vf[42][9]=0.016043;
vf[42][10]=0.018184;
vf[42][11]=0.020354;
vf[42][12]=0.022555;
vf[42][13]=0.024786;
vf[42][14]=0.027047;
vf[42][15]=0.029338;
vf[42][16]=0.031658;
vf[42][17]=0.034006;
vf[42][18]=0.036383;
vf[42][19]=0.038785;
vf[42][20]=0.041213;
vf[42][21]=0.043665;
vf[42][22]=0.046136;
vf[42][23]=0.048625;
vf[42][24]=0.051128;
vf[42][25]=0.053638;
vf[42][26]=0.056148;
vf[43][0]=0.000000;
vf[43][1]=0.000005;
vf[43][2]=0.001801;
vf[43][3]=0.003626;
vf[43][4]=0.005482;
vf[43][5]=0.007368;
vf[43][6]=0.009285;
vf[43][7]=0.011232;
vf[43][8]=0.013209;
vf[43][9]=0.015217;
vf[43][10]=0.017256;
vf[43][11]=0.019325;
vf[43][12]=0.021425;
vf[43][13]=0.023556;
vf[43][14]=0.025716;
vf[43][15]=0.027907;
vf[43][16]=0.030127;
vf[43][17]=0.032376;
vf[43][18]=0.034653;
vf[43][19]=0.036957;
vf[43][20]=0.039287;
vf[43][21]=0.041640;
vf[43][22]=0.044015;
vf[43][23]=0.046407;
vf[43][24]=0.048814;
vf[43][25]=0.051230;
vf[43][26]=0.053648;
vf[44][0]=0.000000;
vf[44][1]=0.000003;
vf[44][2]=0.001703;
vf[44][3]=0.003433;
vf[44][4]=0.005194;
vf[44][5]=0.006985;
vf[44][6]=0.008807;
vf[44][7]=0.010659;
vf[44][8]=0.012542;
vf[44][9]=0.014455;
vf[44][10]=0.016400;
vf[44][11]=0.018375;
vf[44][12]=0.020380;
vf[44][13]=0.022417;
vf[44][14]=0.024483;
vf[44][15]=0.026580;
vf[44][16]=0.028706;
vf[44][17]=0.030861;
vf[44][18]=0.033044;
vf[44][19]=0.035255;
vf[44][20]=0.037492;
vf[44][21]=0.039752;
vf[44][22]=0.042035;
vf[44][23]=0.044336;
vf[44][24]=0.046652;
vf[44][25]=0.048979;
vf[44][26]=0.051309;
vf[45][0]=0.000000;
vf[45][1]=0.000001;
vf[45][2]=0.001614;
vf[45][3]=0.003257;
vf[45][4]=0.004930;
vf[45][5]=0.006633;
vf[45][6]=0.008366;
vf[45][7]=0.010131;
vf[45][8]=0.011926;
vf[45][9]=0.013751;
vf[45][10]=0.015607;
vf[45][11]=0.017494;
vf[45][12]=0.019411;
vf[45][13]=0.021359;
vf[45][14]=0.023337;
vf[45][15]=0.025346;
vf[45][16]=0.027383;
vf[45][17]=0.029450;
vf[45][18]=0.031545;
vf[45][19]=0.033668;
vf[45][20]=0.035816;
vf[45][21]=0.037989;
vf[45][22]=0.040185;
vf[45][23]=0.042399;
vf[45][24]=0.044629;
vf[45][25]=0.046870;
vf[45][26]=0.049116;
vf[46][0]=0.000000;
vf[46][1]=0.000001;
vf[46][2]=0.001533;
vf[46][3]=0.003094;
vf[46][4]=0.004686;
vf[46][5]=0.006308;
vf[46][6]=0.007960;
vf[46][7]=0.009642;
vf[46][8]=0.011355;
vf[46][9]=0.013099;
vf[46][10]=0.014873;
vf[46][11]=0.016677;
vf[46][12]=0.018511;
vf[46][13]=0.020376;
vf[46][14]=0.022272;
vf[46][15]=0.024197;
vf[46][16]=0.026151;
vf[46][17]=0.028134;
vf[46][18]=0.030146;
vf[46][19]=0.032185;
vf[46][20]=0.034251;
vf[46][21]=0.036341;
vf[46][22]=0.038453;
vf[46][23]=0.040585;
vf[46][24]=0.042734;
vf[46][25]=0.044893;
vf[46][26]=0.047059;
vf[47][0]=0.000000;
vf[47][1]=0.000000;
vf[47][2]=0.001458;
vf[47][3]=0.002944;
vf[47][4]=0.004461;
vf[47][5]=0.006007;
vf[47][6]=0.007584;
vf[47][7]=0.009190;
vf[47][8]=0.010826;
vf[47][9]=0.012493;
vf[47][10]=0.014190;
vf[47][11]=0.015917;
vf[47][12]=0.017674;
vf[47][13]=0.019461;
vf[47][14]=0.021278;
vf[47][15]=0.023125;
vf[47][16]=0.025001;
vf[47][17]=0.026905;
vf[47][18]=0.028838;
vf[47][19]=0.030799;
vf[47][20]=0.032785;
vf[47][21]=0.034797;
vf[47][22]=0.036830;
vf[47][23]=0.038884;
vf[47][24]=0.040955;
vf[47][25]=0.043037;
vf[47][26]=0.045127;
vf[48][0]=0.000000;
vf[48][1]=0.000000;
vf[48][2]=0.001388;
vf[48][3]=0.002806;
vf[48][4]=0.004252;
vf[48][5]=0.005728;
vf[48][6]=0.007234;
vf[48][7]=0.008769;
vf[48][8]=0.010335;
vf[48][9]=0.011929;
vf[48][10]=0.013554;
vf[48][11]=0.015208;
vf[48][12]=0.016893;
vf[48][13]=0.018607;
vf[48][14]=0.020350;
vf[48][15]=0.022123;
vf[48][16]=0.023925;
vf[48][17]=0.025756;
vf[48][18]=0.027614;
vf[48][19]=0.029500;
vf[48][20]=0.031412;
vf[48][21]=0.033348;
vf[48][22]=0.035308;
vf[48][23]=0.037287;
vf[48][24]=0.039283;
vf[48][25]=0.041293;
vf[48][26]=0.043309;
vf[49][0]=0.000000;
vf[49][1]=0.000000;
vf[49][2]=0.001324;
vf[49][3]=0.002677;
vf[49][4]=0.004059;
vf[49][5]=0.005470;
vf[49][6]=0.006909;
vf[49][7]=0.008378;
vf[49][8]=0.009876;
vf[49][9]=0.011404;
vf[49][10]=0.012961;
vf[49][11]=0.014547;
vf[49][12]=0.016163;
vf[49][13]=0.017808;
vf[49][14]=0.019482;
vf[49][15]=0.021186;
vf[49][16]=0.022918;
vf[49][17]=0.024678;
vf[49][18]=0.026466;
vf[49][19]=0.028281;
vf[49][20]=0.030123;
vf[49][21]=0.031988;
vf[49][22]=0.033877;
vf[49][23]=0.035786;
vf[49][24]=0.037711;
vf[49][25]=0.039651;
vf[49][26]=0.041598;
vf[50][0]=0.000000;
vf[50][1]=0.000000;
vf[50][2]=0.001265;
vf[50][3]=0.002558;
vf[50][4]=0.003879;
vf[50][5]=0.005228;
vf[50][6]=0.006606;
vf[50][7]=0.008013;
vf[50][8]=0.009449;
vf[50][9]=0.010913;
vf[50][10]=0.012407;
vf[50][11]=0.013929;
vf[50][12]=0.015480;
vf[50][13]=0.017061;
vf[50][14]=0.018670;
vf[50][15]=0.020307;
vf[50][16]=0.021973;
vf[50][17]=0.023667;
vf[50][18]=0.025389;
vf[50][19]=0.027137;
vf[50][20]=0.028911;
vf[50][21]=0.030709;
vf[50][22]=0.032531;
vf[50][23]=0.034372;
vf[50][24]=0.036231;
vf[50][25]=0.038103;
vf[50][26]=0.039984;
}

void Voigt::vstart(double dmp){
  int i;
  double xparb = 25.*dmp + 1.;
  lastv = 1;
  if (xparb < 1.) xparb = 1.;
  if (xparb > 25.99999) return;
  lastv = 2;
  if (xparb == xlast) return;
  int ib = int(xparb);
  double xd = double(xparb - double(ib));
//  printf("* xparb = %lf lastv = %i ib = %i \n",xparb,lastv,ib);
  for(i=1;i<=50;i++) {
    vt[i] = vf[i][ib] + xd*(vf[i][ib+1] - vf[i][ib]);
//    printf("** %lf  %i %lf vf=%lf\n",xd,i,vt[i],vf[i][ib]);
  };
  ds[1] = 2.0 * (vt[2] - vt[1]);
  for(i=2;i<=49;i++)
    ds[i] = vt[i+1] - 2.0 * vt[i] + vt[i-1];
  double tb = 0.1809017;
  dm[1] = ds[1] - tb*2.0*(ds[2]-ds[1]);
  for(i=2;i<=48;i++)
    dm[i] = ds[i] - tb*(ds[i+1]-2.0*ds[i]+ds[i-1]);
  double b = (xparb - 1.0)*0.04;
  double a = bgg[2*ib-1] + xd*(bgg[2*ib+1]-bgg[2*ib-1]);
  double pt = p[ib] + xd * (p[ib+1]-p[ib]);
  double aa = a*a;
  c = 2*a + b*b;
  ca = b*pt*0.6366198;
  cb = 8*a*ca;
  cc = cb*(13.*a - c);
  cd = ca*32*aa*(58*a - 9*c);
  ce = ca*64*aa*(655*aa - 150*a*c + 3*c*c);
  ulim = pow(1e6*cb,0.25);
  if (ulim < 4.7) ulim = 4.7;
}

double Voigt::voigt(double uu,double wdth){
  double u = uu/(wdth!=0.?wdth/2:1.);
  if (lastv != 2) return 1./(1. + u*u);
  if (u == 0.) return 1./(1. + u*u);
  if (u < 0.) u = -u;
  if (u >= ulim) return ca/(u*u + c);
  if (u >= 4.7) {
    double g = 1./(u*u + c);
    return g*(ca + g*(cb + g*(cc + g*(cd + g*ce))));
  };
  double ut = 10.*u;
  int n = int(ut);
  double zm = ut - double(n);
  return vt[n+1]+zm*(vt[n+2]-vt[n+1]+(zm-1.)*
          ((zm+1.)*dm[n+2]+(2.-zm)*dm[n+1])/6.);
}

double Voigt::operator()(double h,double w,double wdth,double dmp){
  if (dmp != old_dmp){
    vstart(dmp);
    old_dmp = dmp;
  };
  return h*voigt(w,wdth);
}