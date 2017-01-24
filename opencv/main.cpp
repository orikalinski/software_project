#include "main_aux.h"

struct sp_point_t {
    double *data;
    int index;
    int dim;
};

int main(){
    printf("Enter images directory path:\n");
    std::string imagesDirectory;
    std::getline (std::cin, imagesDirectory);

    printf("Enter images prefix:\n");
    std::string imagesPrefix;
    std::getline (std::cin, imagesPrefix);

    printf("Enter number of images:\n");
    int numberOfImages;
    std::cin >> numberOfImages;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (numberOfImages < 1)
        printf("An error occurred - invalid number of images\n"); // free all allocations

    printf("Enter images suffix:\n");
    std::string imagesSuffix;
    std::getline (std::cin, imagesSuffix);

    printf("Enter number of bins:\n");
    int numberOfBins;
    std::cin >> numberOfBins;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (numberOfBins < 1 || numberOfBins > 255)
        printf("An error occurred - invalid number of bins\n"); // free al allocations

    printf("Enter number of features:\n");
    int numberOfFeaturesToExtract;
    std::cin >> numberOfFeaturesToExtract;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (numberOfFeaturesToExtract < 1)
        printf("An error occurred - invalid number of features\n"); // free all allocations

    // calculate RGB histogram and siftDesrcriptors for each image.
    int *nFeatures = (int *)malloc(sizeof(int));
    SPPoint ***RgbHist = (SPPoint ***)calloc(numberOfImages, sizeof(SPPoint **));
    SPPoint ***descriptors = (SPPoint ***)calloc(numberOfImages, sizeof(SPPoint **));
    int *nFeaturesPerImage = (int *)calloc(numberOfImages, sizeof(int));
    std::string imagePath;
    for (int i=0; i < numberOfImages; i++){
        imagePath = imagesDirectory + imagesPrefix + std::to_string(i) + imagesSuffix;
        cout << imagePath << "\n";
        RgbHist[i] = spGetRGBHist(imagePath.c_str(), i, numberOfBins);
        descriptors[i] = spGetSiftDescriptors(imagePath.c_str(), i, numberOfFeaturesToExtract, nFeatures);
        nFeaturesPerImage[i] = *nFeatures;
    }

    printf("Enter a query image or # to terminate:\n");
    std::string queryImagePath;
    std::getline (std::cin, queryImagePath);
    while (queryImagePath != "#") {
        SPPoint **QRGBHist = spGetRGBHist(queryImagePath.c_str(), -1, numberOfBins);
        SPPoint **QDesc = spGetSiftDescriptors(queryImagePath.c_str(), -1, numberOfFeaturesToExtract, nFeatures);
        double *queryRGBtoImagesDistance = (double *) calloc(numberOfImages, sizeof(double));
        for (int i = 0; i < numberOfImages; i++)
            queryRGBtoImagesDistance[i] = spRGBHistL2Distance(QRGBHist, RgbHist[i]);
        printf("Nearest images using global descriptors:\n");
        printBestFive(queryRGBtoImagesDistance, numberOfImages, 1);
        double *imagesHits = (double *) calloc(numberOfImages, sizeof(double));
        int *bestSiftDistanceImageIndexes;
        for (int i = 0; i < *nFeatures; i++) {
            bestSiftDistanceImageIndexes = spBestSIFTL2SquaredDistance(5, QDesc[i], descriptors,
                                                                       numberOfImages, nFeaturesPerImage);
            for (int j = 0; j < 5; j++)
                imagesHits[bestSiftDistanceImageIndexes[j]]++;
        }
        printf("Nearest images using local descriptors:\n");
        printBestFive(imagesHits, numberOfImages, -1);
        std::getline (std::cin, queryImagePath);
    }
    printf("Exiting...\n");
}

