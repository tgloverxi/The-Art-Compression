
#include "stats.h"

stats::stats(PNG & im){
    unsigned int x = im.width();
    unsigned int y = im.height();
//    sumHueX.clear();
    sumHueX.resize(x,vector<double>(y));
//    sumHueY.clear();
    sumHueY.resize(x,vector<double>(y));
//    sumSat.clear();
    sumSat.resize(x,vector<double>(y));
//    sumLum.clear();
    sumLum.resize(x,vector<double>(y));
//    hist.clear();
    hist.resize(im.width(),vector<vector<int>>(im.height(),vector<int>(36)));
    for(unsigned int i=0;i<x;i++){
        for(unsigned int j=0;j<y;j++){
            HSLAPixel *p=im.getPixel(i,j);
            double hueX = cos(PI*(p->h)/180);
            double hueY = sin(PI*(p->h)/180);
            if (i == 0 && j ==0) {

                sumHueX[i][j] = hueX;
                sumHueY[i][j]= hueY;
                sumSat[i][j] = p->s;
                sumLum[i][j] = p->l;
//                hist[i][j]=buildHist((0,0),(i,j));
                hist[i][j][p->h / 10]++;
            } else if (i == 0) {
                sumHueX[i][j] = sumHueX[i][j - 1] + hueX;
                sumHueY[i][j] = sumHueY[i][j - 1] + hueY;
                sumSat[i][j] = sumSat[i][j - 1] + p->s;
                sumLum[i][j] = sumLum[i][j - 1] + p->l;
//
//                hist[i][j]=buildHist((0,0),(i,j));
                hist[i][j]=hist[i][j-1];
                hist[i][j][p->h / 10]++;
            } else if (j == 0) {
                sumHueX[i][j] = sumHueX[i - 1][j] + hueX;
                sumHueY[i][j] = sumHueY[i - 1][j] + hueY;
                sumSat[i][j] = sumSat[i - 1][j] + p->s;
                sumLum[i][j] = sumLum[i - 1][j] + p->l;

//                buildHist((0,0),(i,j));
//                hist[i][j]=buildHist((0,0),(i,j));
                hist[i][j]=hist[i-1][j];
                hist[i][j][p->h / 10]++;

            } else {
                sumHueX[i][j] = sumHueX[i][j - 1] + sumHueX[i - 1][j] - sumHueX[i - 1][j - 1] + hueX;
                sumHueY[i][j] = sumHueY[i][j - 1] + sumHueY[i - 1][j] - sumHueY[i - 1][j - 1] + hueY;
                sumSat[i][j] = sumSat[i][j - 1] + sumSat[i - 1][j] - sumSat[i - 1][j - 1] + p->s;
                sumLum[i][j] = sumLum[i][j - 1] + sumLum[i - 1][j] - sumLum[i - 1][j - 1] + p->l;

//                buildHist((0,0),(i,j));
//                hist[i][j]=buildHist((0,0),(i,j));
//                hist[i][j]=transform(hist[i-1][j].begin(),hist[i-1][j].end(),hist[i][j-1].begin(),hist[i][j-1].end(),plus<int>());
//                hist[i][j]=transform(hist[i][j].begin(),hist[i][j].end(),hist[i-1][j-1].begin(),hist[i-1][j-1].end(),minus<int>());
//                hist[i][j]=hist[i][j-1]+hist[i-1][j]-hist[i-1][j-1];
                for(int k=0;k<36;k++){
                    hist[i][j][k]=hist[i-1][j][k]+hist[i][j-1][k]-hist[i-1][j-1][k];
                }
                hist[i][j][p->h / 10]++;
            }


        }
    }

    
    
//    int x = im.width();
//    int y = im.height();
//    sumHueX.resize(x, vector<double>(y));
//
//    for (int i = 0;i<x;i++){
//        for(int j = 0;j<y; j++){
//            HSLAPixel *pixel = im.getPixel(i,j);
//            double hueX = cos(PI*(pixel->h)/180);
//            if (i == 0 && j ==0){
//                sumHueX[i][j]=hueX;}
//            else if (i == 0){
//                sumHueX[i][j] = sumHueX[i][j-1]+hueX;}
//            else if (j == 0){
//                sumHueX[i][j]= sumHueX[i-1][j]+hueX;}
//
//            else{
//
//                sumHueX[i][j]=sumHueX[i][j-1]+sumHueX[i-1][j]-sumHueX[i-1][j-1]+hueX;
//            }
//        }
//    }
//
//    sumHueY.resize(x, vector<double>(y));
//
//    for (int i = 1;i<x;i++){
//        for(int j = 1;j<y; j++){
//            HSLAPixel *pixel = im.getPixel(i,j);
//            double hueY = sin(PI*(pixel->h)/180);
//            if (i == 0 && j ==0){
//                sumHueY[i][j]=hueY;}
//            else if (i == 0){
//                sumHueY[i][j]= sumHueY[i][j-1]+hueY;}
//            else if (j == 0){
//                sumHueY[i][j]= sumHueY[i-1][j]+hueY;}
//            else
//            {sumHueY[i][j]=sumHueY[i][j-1]+sumHueY[i-1][j]-sumHueY[i-1][j-1]+hueY;
//            }
//        }
//    }
//
//
//    sumSat.resize(x, vector<double>(y));
//    sumSat[0][0]=im.getPixel(0,0)->s;
//    for (int i = 1;i<x;i++){
//        sumSat[i][0]= sumSat[i-1][0]+im.getPixel(i,0)->s;}
//    for(int j = 1;j<y; j++){
//        sumSat[0][j] = sumSat[0][j-1]+im.getPixel(0,j)->s;
//    }
//    for (int i = 1;i<x;i++){
//        for(int j = 1;j<y; j++){
//            HSLAPixel *pixel = im.getPixel(i,j);
//            double sat = pixel->s;
//            sumSat[i][j]=sumSat[i][j-1]+sumSat[i-1][j]-sumSat[i-1][j-1]+sat;
//        }
//    }
//
//    sumLum.resize(x, vector<double>(y));
//    //sumLum[0][0]=im.getPixel(0,0)->l;
//    //for (int i = 1;i<x;i++){
//    //     sumLum[i][0]= sumLum[i-1][0]+im.getPixel(i,0)->l;}
//    // //for(int j = 1;j<y; j++){
//    //     sumLum[0][j] = sumLum[0][j-1]+im.getPixel(0,j)->l;
//    for (int i = 0;i<x;i++){
//        for(int j = 0;j<y; j++){
//            HSLAPixel *pixel = im.getPixel(i,j);
//            double lum = pixel->l;
//            if (i == 0 && j ==0){
//                sumLum[i][j]=lum;}
//            else if (i == 0){
//                sumLum[i][j] = sumLum[i][j-1]+lum;
//            }
//            else if (j == 0){
//                sumLum[i][j]= sumLum[i-1][j]+lum;}
//            else{
//                sumLum[i][j]=sumLum[i][j-1]+sumLum[i-1][j]-sumLum[i-1][j-1]+lum;
//            }
//        }
//    }
//    /* hist[i][j][k]: hist[i][j] contains a histogram of the hue values
//     *   0 to 360 into bins of width 10, over the pixels in the rectangle
//     *   defined by (0,0) through (i,j). For example, hist[i][j][k] contains
//     *   the number of pixels whose hue value h, is: k*10 <= h < (k+1)*10.
//     */
//    hist.resize(x, vector<vector<int>> (y, vector<int>(36)));
//    for (int k = 0; k<36;k++){
//        HSLAPixel *pixel = im.getPixel(0,0);
//        int bin = pixel->h/10;
//        if (bin == k){
//            hist[0][0][k]=1;
//        }
//        else{
//            hist[0][0][k]=0;
//        }
//    }
//
//    for (int i = 1;i<x;i++){
//        for (int k = 0; k<36;k++){
//            HSLAPixel *pixel = im.getPixel(i,0);
//            int bin = pixel->h/10;
//            if (bin == k){
//                hist[i][0][k]=hist[i-1][0][k]+1;
//            }
//            else{
//                hist[i][0][k]=hist[i-1][0][k];
//            }
//
//        }
//    }
//
//    for (int j = 1;j<y;j++){
//        for (int k = 0; k<36;k++){
//            HSLAPixel *pixel = im.getPixel(0,j);
//            int bin = pixel->h/10;
//            if (bin == k){
//                hist[0][j][k]=hist[0][j-1][k]+1;
//            }
//            else{
//                hist[0][j][k]=hist[0][j-1][k];
//            }
//
//        }
//    }
//
//    for (int i = 1;i<x;i++){
//        for(int j = 1;j<y; j++){
//            for (int k = 0; k<36;k++){
//                HSLAPixel *pixel = im.getPixel(i,j);
//                int bin = pixel->h/10;
//                if(bin == k){
//                    hist[i][j][k]=hist[i-1][j][k]+hist[i][j-1][k]-hist[i-1][j-1][k]+1;
//                }
//                else{
//                    hist[i][j][k]= hist[i-1][j][k]+hist[i][j-1][k]-hist[i-1][j-1][k];
//                }
//            }
//
//
//
//
//
//        }
//    }
//
//
//
//
//

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

    long recA =(lr.first-ul.first+1)*(lr.second-ul.second+1);
    return recA;
    

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    HSLAPixel avg;
    long area = rectArea(ul, lr);
    double amount=(double)area;
    double ah=0.0;
    double as=0.0;
    double al=0.0;
    double avHx=0.0;
    double avHy=0.0;
    int a=ul.first;
    int b=ul.second;
    int i=lr.first;
    int j=lr.second;
    if(a==0&&b==0){
    avHx=sumHueX[i][j]/amount;
    avHy=sumHueY[i][j]/amount;
    ah=atan2(avHy,avHx)*180/PI;
    as=sumSat[i][j]/rectArea(ul,lr);
    al=sumLum[i][j]/rectArea(ul,lr);
    }else if(a==0){
       
            avHx=(sumHueX[i][j]-sumHueX[i][b-1])/amount;
            avHy=(sumHueY[i][j]-sumHueY[i][b-1])/amount;
            ah=atan2(avHy, avHx) * 180/PI;
            as=(sumSat[i][j]-sumSat[i][b-1])/rectArea(ul,lr);
            al=(sumLum[i][j]-sumLum[i][b-1])/rectArea(ul,lr);
    }else if(b==0){
        avHx=(sumHueX[i][j]-sumHueX[a-1][j])/amount;
        avHy=(sumHueY[i][j]-sumHueY[a-1][j])/amount;
        ah=atan2(avHy,avHx)*180/PI;
        as=(sumSat[i][j]-sumSat[a-1][j])/rectArea(ul,lr);
        al=(sumLum[i][j]-sumLum[a-1][j])/rectArea(ul,lr);
    }else{
        avHx=(sumHueX[i][j]-sumHueX[a-1][j]-sumHueX[i][b-1]+sumHueX[a-1][b-1])/amount;
        avHy=(sumHueY[i][j]-sumHueY[a-1][j]-sumHueY[i][b-1]+sumHueY[a-1][b-1])/amount;
//        ah=atan2(avHy,avHx)*180/PI;
//        double sumLX = sumHueX[ul.first-1][lr.second];
//        double sumLY = sumHueY[ul.first-1][lr.second];
//        double sumULX = sumHueX[ul.first-1][ul.second-1];
//        double sumULY = sumHueY[ul.first-1][ul.second-1];
//        double sumUX = sumHueX[lr.first][ul.second-1];
//        double sumUY = sumHueY[lr.first][ul.second-1];
//        double areaX = sumHueX[i][j]- sumLX - sumUX + sumULX;
//        double areaY = sumHueY[i][j] - sumLY - sumUY + sumULY;
//        avHx = areaX / amount;
//        avHy= areaY / amount;
         ah=atan2(avHy,avHx)*180/PI;
        as=(sumSat[i][j]-sumSat[a-1][j]-sumSat[i][b-1]+sumSat[a-1][b-1])/rectArea(ul,lr);
        al=(sumLum[i][j]-sumLum[a-1][j]-sumLum[i][b-1]+sumLum[a-1][b-1])/rectArea(ul,lr);
        
    }
    while (ah<0){
        ah = ah + 360;
    }
    avg=HSLAPixel(ah,as,al);
        
    return avg;
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){
 vector<int> his;
    his.resize(36);
    int numBin = 36;
    int a=ul.first;
    int b=ul.second;
    int i=lr.first;
    int j=lr.second;
    for(int k = 0; k < numBin; k++){
        if(a==0&&b==0){
            his[k]=hist[i][j][k];
            //pushbakc??
        }else if(a==0){
            his[k]=hist[i][j][k]-hist[i][b-1][k];
        }else if (b==0){
             his[k]=hist[i][j][k]-hist[a-1][j][k];
        }else{
             his[k]=hist[i][j][k]-hist[a-1][j][k]-hist[i][b-1][k]+hist[a-1][b-1][k];
        }
    
    
}
    return his;
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

/* your code here */

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    long area=rectArea(ul,lr);
//   vector<int> his(36, 0);
    
    vector<int> his=buildHist(ul,lr);
    
    
    return entropy(his,area);

}



