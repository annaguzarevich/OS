#include <iostream>
#include <thread>
#include <ctime>
#include <vector>
#include <mutex>
#include "buffered_channel.h"

typedef std::tuple<int, int, int, int, int, int, int> tuple;

std::mutex g_lock;

const int N = 5;
int **A = new int *[N];
int **B = new int *[N];
int **result = new int *[N];

const int BUFFER_SIZE = N * N * N;

void GenerateAndDisplayMatrices() {
    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        result[i] = new int[N];
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = 0;
        }
    }

    srand(time(NULL));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << A[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << B[i][j] << ' ';
        }
        std::cout << '\n';
    }
}


void Multiply(int AblockWidth, int AblockHeight, int BblockWidth, int AstartRow,
              int AstartColumn, int BstartRow, int BstartColumn) {

    for (int i = 0; i < AblockHeight; ++i) {
        for (int j = 0; j < BblockWidth; ++j) {
            for (int k = 0; k < AblockWidth; ++k) {
                g_lock.lock();
                result[AstartRow + i][BstartColumn + j] +=
                        A[AstartRow + i][AstartColumn + k] * B[BstartRow + k][BstartColumn + j];
                g_lock.unlock();
            }
        }
    }
}


int main() {

    GenerateAndDisplayMatrices();

    for (int blockSize = 1; blockSize <= N; ++blockSize) {
        int numberOfBlocks = N / blockSize;
        int lastBlockSize = blockSize;

        int AblockWidth;
        int AblockHeight;
        int BblockWidth;

        if (N % blockSize != 0) {
            numberOfBlocks++;
            lastBlockSize = N % blockSize;
        }

        std::vector<std::thread> ThreadVector;
        for (int number_of_threads = numberOfBlocks;
             number_of_threads <= numberOfBlocks * numberOfBlocks * numberOfBlocks; number_of_threads *= 2) {

            BufferedChannel<tuple> bufferedChannel(BUFFER_SIZE);

            auto start = std::chrono::steady_clock::now();


//            int numberOfIterations = numberOfBlocks * numberOfBlocks * numberOfBlocks;
//            int currentIteration = 1;
//            for (int i = 0; i < number_of_threads; ++i) {
//                ThreadVector.emplace_back([&bufferedChannel, numberOfIterations, &currentIteration, i]() {
//                    while (currentIteration <= numberOfIterations) {
//                        std::pair data = bufferedChannel.Recv();
//                        //std::cout << "received in thr number " << i << '\n';
//                        //std::cout<<"iter "<<currentIteration<<' '<<data.second<<'\n';
//                        if (data.second) {
//                            Multiply(std::get<0>(data.first),
//                                     std::get<1>(data.first), std::get<2>(data.first), std::get<3>(data.first),
//                                     std::get<4>(data.first), std::get<5>(data.first), std::get<6>(data.first));
//                            g_lock.lock();
//                            currentIteration++;
//                            g_lock.unlock();
//                        } //else std::cout<<"iter "<<currentIteration<<"false"<<'\n';
//                        if (currentIteration > numberOfIterations) bufferedChannel.Close();
//                    }
//                });
//            }

            for (int i = 0; i < numberOfBlocks; ++i) {
                for (int j = 0; j < numberOfBlocks; ++j) {
                    for (int k = 0; k < numberOfBlocks; ++k) {

                        if (i == numberOfBlocks - 1) AblockHeight = lastBlockSize;
                        else AblockHeight = blockSize;

                        if (j == numberOfBlocks - 1) BblockWidth = lastBlockSize;
                        else BblockWidth = blockSize;

                        if (k == numberOfBlocks - 1) AblockWidth = lastBlockSize;
                        else AblockWidth = blockSize;


                        tuple parameters = std::make_tuple(AblockWidth, AblockHeight,
                                                           BblockWidth,
                                                           i * blockSize,
                                                           k * blockSize,
                        k * blockSize, j * blockSize);

                        ThreadVector.emplace_back([&bufferedChannel, parameters](){
                            bufferedChannel.Send(parameters);
                        });

                        //std::cout << "sent " << i << ' ' << j << ' ' << k << '\n';

//                    std::thread thrSend = std::thread(Multiply, AblockWidth, AblockHeight,
//                                                  BblockWidth, BblockHeight,
//                                                  i * blockSize,
//                                                  k * blockSize,
//                                                  k * blockSize, j * blockSize);
//                    ThreadVector.push_back(move(thrSend));
                    }
                }
            }

            bufferedChannel.Close();

            for (int i = 0; i < number_of_threads; ++i) {
                ThreadVector.emplace_back([&bufferedChannel, i]() {
                    while (true) {
                        std::pair data = bufferedChannel.Recv();
                        //std::cout << "received in thr number " << i << '\n';
                        if (data.second) {
                            Multiply(std::get<0>(data.first),
                                     std::get<1>(data.first), std::get<2>(data.first), std::get<3>(data.first),
                                     std::get<4>(data.first), std::get<5>(data.first), std::get<6>(data.first));
                        }
                        else break;
                    }
                });
            }


//            int numberOfIterations = numberOfBlocks * numberOfBlocks * numberOfBlocks;
//            int currentIteration = 1;
//            for (int i = 0; i < number_of_threads; ++i) {
//                ThreadVector.emplace_back([&bufferedChannel, numberOfIterations, &currentIteration, i]() {
//                    while (currentIteration <= numberOfIterations) {
//                        std::pair data = bufferedChannel.Recv();
//                        //std::cout << "received in thr number " << i << '\n';
//                        //std::cout<<"iter "<<currentIteration<<' '<<data.second<<'\n';
//                        if (data.second) {
//                            Multiply(std::get<0>(data.first),
//                                     std::get<1>(data.first), std::get<2>(data.first), std::get<3>(data.first),
//                                     std::get<4>(data.first), std::get<5>(data.first), std::get<6>(data.first));
//                            g_lock.lock();
//                            currentIteration++;
//                            g_lock.unlock();
//                        } //else std::cout<<"iter "<<currentIteration<<"false"<<'\n';
//                        if (currentIteration > numberOfIterations) bufferedChannel.Close();
//                    }
//                });
//            }


            for (auto &thr: ThreadVector) {
                thr.join();
            }

            auto end = std::chrono::steady_clock::now();
            int time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            ThreadVector.clear();

            std::cout << '\n' << "BlockSize = " << blockSize << ", NumberOfThreads = " << number_of_threads
                      << ", duration = " << time << '\n';
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    std::cout << result[i][j] << ' ';
                }
                std::cout << '\n';
            }

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    result[i][j] = 0;
                }
            }
        }
    }

    return 0;
}
