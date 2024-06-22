#ifndef SONG_ANALIZER_H
#define SONG_ANALIZER_H
#include <iostream>
#include <cmath>
#include <complex>
#include <fftw3.h>
#include <sndfile.h>
#include <vector>
#include <glm/glm.hpp>

using namespace std;




inline static double equalLoudnessContour(double frequency) {
    // These are made up values, you would need to replace these with the actual data points
    // from the Fletcher-Munson curve.
    const double fletcherMunsonData[] = {
    20, 0.86,     // Frequency: 20 Hz, SPL: 86 dB
    25, 0.76,     // Frequency: 25 Hz, SPL: 76 dB
    31.5, 0.67,   // Frequency: 31.5 Hz, SPL: 67 dB
    40, 0.59,     // Frequency: 40 Hz, SPL: 59 dB
    50, 0.53,     // Frequency: 50 Hz, SPL: 53 dB
    63, 0.48,     // Frequency: 63 Hz, SPL: 48 dB
    80, 0.44,     // Frequency: 80 Hz, SPL: 44 dB
    100, 0.40,    // Frequency: 100 Hz, SPL: 40 dB
    125, 0.37,    // Frequency: 125 Hz, SPL: 37 dB
    160, 0.34,    // Frequency: 160 Hz, SPL: 34 dB
    200, 0.31,    // Frequency: 200 Hz, SPL: 31 dB
    250, 0.28,    // Frequency: 250 Hz, SPL: 28 dB
    315, 0.25,    // Frequency: 315 Hz, SPL: 25 dB
    400, 0.22,    // Frequency: 400 Hz, SPL: 22 dB
    500, 0.20,    // Frequency: 500 Hz, SPL: 20 dB
    630, 0.18,    // Frequency: 630 Hz, SPL: 18 dB
    800, 0.16,    // Frequency: 800 Hz, SPL: 16 dB
    1000, 0.15,   // Frequency: 1000 Hz, SPL: 15 dB
    1250, 0.13,   // Frequency: 1250 Hz, SPL: 13 dB
    1600, 0.12,   // Frequency: 1600 Hz, SPL: 12 dB
    2000, 0.11,   // Frequency: 2000 Hz, SPL: 11 dB
    2500, 0.10,   // Frequency: 2500 Hz, SPL: 10 dB
    3150, 0.09,   // Frequency: 3150 Hz, SPL: 9 dB
    4000, 0.08,   // Frequency: 4000 Hz, SPL: 8 dB
    5000, 0.07,   // Frequency: 5000 Hz, SPL: 7 dB
    6300, 0.06,   // Frequency: 6300 Hz, SPL: 6 dB
    8000, 0.05,   // Frequency: 8000 Hz, SPL: 5 dB
    10000, 0.04   // Frequency: 10000 Hz, SPL: 4 dB
    };
    ;


    // Find the two data points that the frequency falls between
    int index = 0;
    while (index < sizeof(fletcherMunsonData) / sizeof(double) - 2 && frequency > fletcherMunsonData[index]) {
        index += 2;
    }

    // Interpolate between the two data points
    double freq1 = fletcherMunsonData[index];
    double adjustment1 = fletcherMunsonData[index + 1];
    double freq2 = fletcherMunsonData[index + 2];
    double adjustment2 = fletcherMunsonData[index + 3];

    double ratio = (frequency - freq1) / (freq2 - freq1);
    return adjustment1 + ratio * (adjustment2 - adjustment1);
}

enum sampleType {
    BASS,
    MID,
    CLAP,
    SKIP
};

struct SongSample {
    glm::vec3 bass;
    glm::vec3 mid;
    glm::vec3 high;
    sampleType type;
};

// Function to generate a Hamming window
inline static std::vector<double> generateHammingWindow(int N) {
    std::vector<double> window(N);
    for (int n = 0; n < N; ++n) {
        window[n] = 0.54 - 0.46 * cos(2 * 3.1415 * n / (N - 1));
    }
    return window;
}

// Function to apply the Hamming window to an audio chunk
inline static void applyWindow(std::vector<double>& chunk, const std::vector<double>& window) {
    for (size_t i = 0; i < chunk.size(); ++i) {
        chunk[i] *= window[i];
    }
}

class SongAnalizer {
public:

    

    inline static void parseSong(double chunkDuration, const char* filename, vector<SongSample>& result) {


        SF_INFO info;
        SNDFILE* file = sf_open(filename, SFM_READ, &info);
        if (!file) {
            cerr << "Error opening file: " << sf_strerror(file) << endl;
        }

        int samplesPerChunk = static_cast<int>(info.samplerate * chunkDuration);

        double* samples = new double[samplesPerChunk];

        double min_bassMean = std::numeric_limits<double>::max(); // Initialize to maximum possible double
        double max_bassMean = std::numeric_limits<double>::min(); // Initialize to minimum possible double

        double min_highMean = std::numeric_limits<double>::max(); // Initialize to maximum possible double
        double max_highMean = std::numeric_limits<double>::min(); // Initialize to minimum possible double


        double min_midMean = std::numeric_limits<double>::max(); // Initialize to maximum possible double
        double max_midMean = std::numeric_limits<double>::min(); // Initialize to minimum possible double


        for (int chunkStart = 0; chunkStart < info.frames; chunkStart += samplesPerChunk) {

            sf_seek(file, chunkStart, SEEK_SET);
            sf_read_double(file, samples, samplesPerChunk);
            double timestamp = static_cast<double>(chunkStart) / info.samplerate;


            fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * samplesPerChunk);
            for (int i = 0; i < samplesPerChunk; ++i) {
                in[i][0] = samples[i];
                in[i][1] = 0;
            }

            fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * samplesPerChunk);
            fftw_plan forward = fftw_plan_dft_1d(samplesPerChunk, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

            fftw_execute(forward);

            const double frequencyStep = info.samplerate / static_cast<double>(samplesPerChunk);

            double bassSum = 0, midSum = 0, highSum = 0;
            int bassCount = 0, midCount = 0, highCount = 0;


            double maxBassMagnitude = 0, maxMidMagnitude = 0, maxHighMagnitude = 0;
            double predominantBassFrequency = 0, predominantMidFrequency = 0, predominantHighFrequency = 0;

            for (int i = 0; i < samplesPerChunk / 2; ++i) {
                double frequency = i * frequencyStep;

                double real = out[i][0];
                double imag = out[i][1];

                double magnitude = sqrt(real * real + imag * imag);

                double adjustedMagnitude = magnitude * equalLoudnessContour(frequency);
                magnitude = adjustedMagnitude;

                if (frequency >= 20 && frequency <= 250) {
                    bassSum += magnitude;
                    bassCount++;
                    if (magnitude > maxBassMagnitude) {
                        maxBassMagnitude = magnitude;
                        predominantBassFrequency = frequency;
                    }
                }
                else if (frequency > 250 && frequency <= 4000) {
                    midSum += magnitude;
                    midCount++;
                    if (magnitude > maxMidMagnitude) {
                        maxMidMagnitude = magnitude;
                        predominantMidFrequency = frequency;
                    }
                }
                else if (frequency > 4000 && frequency <= 10000) {
                    highSum += magnitude;
                    highCount++;
                    if (magnitude > maxHighMagnitude) {
                        maxHighMagnitude = magnitude;
                        predominantHighFrequency = frequency;
                    }
                }
            }

            double bassMean = bassCount > 0 ? bassSum / bassCount : 0;

            // Update min and max bassMean
            if (bassMean < min_bassMean) {
                min_bassMean = bassMean;
            }
            if (bassMean > max_bassMean) {
                max_bassMean = bassMean;
            }

            // Normalize bassMean
            double normalized_bassMean = (bassMean - min_bassMean) / (max_bassMean - min_bassMean);

            double midMean = midCount > 0 ? midSum / midCount : 0;

            // Update min and max midMean
            if (midMean < min_midMean) {
                min_midMean = midMean;
            }
            if (midMean > max_midMean) {
                max_midMean = midMean;
            }

            double normalized_midMean = (midMean - min_midMean) / (max_midMean - min_midMean);

            double highMean = highCount > 0 ? highSum / highCount : 0;

            // Update min and max highMean
            if (highMean < min_highMean) {
                min_highMean = highMean;
            }
            if (highMean > max_highMean) {
                max_highMean = highMean;
            }

            double normalized_highMean = (highMean - min_highMean) / (max_highMean - min_highMean);




            // Normalize highMean

            double normalizedPredominantBassFrequency = 0;
            double normalizedPredominantMidFrequency = 0;
            double normalizedPredominantHighFrequency = 0;



            if (predominantBassFrequency != 0 || predominantMidFrequency != 0 || predominantHighFrequency != 0)
            {
                normalizedPredominantBassFrequency = (predominantBassFrequency - 20) / (250 - 20);
                normalizedPredominantMidFrequency = (predominantMidFrequency - 250) / (4000 - 250);
                normalizedPredominantHighFrequency = (predominantHighFrequency - 4000) / (10000 - 4000);
            }

            SongSample sample;



            sample.bass = glm::vec3(normalized_bassMean, normalizedPredominantBassFrequency, -2137);
            sample.mid = glm::vec3(normalized_midMean, -1, -6969);
            sample.high = glm::vec3(normalized_highMean, 1, -420);



            result.push_back(sample);



            fftw_destroy_plan(forward);
            fftw_free(in);
            fftw_free(out);

        }


        delete[] samples;
        sf_close(file);
    }


    inline static void songLenth(const char* filename) {

        SF_INFO info;
        SNDFILE* file = sf_open(filename, SFM_READ, &info);
        if (!file) {
            cerr << "Error opening file: " << sf_strerror(file) << endl;
            return;
        }
    }

    inline static void testparseSong(double chunkDuration, const char* filename, vector<SongSample>& result) {
        // Load the audio file
        SF_INFO sfinfo;
        SNDFILE* file = sf_open(filename, SFM_READ, &sfinfo);

        double duration = static_cast<double>(sfinfo.frames) / sfinfo.samplerate;

        // Calculate the number of samples per chunk
        int chunk_size = (chunkDuration * sfinfo.samplerate);
        std::vector<double> hammingWindow = generateHammingWindow(chunk_size);
        // Prepare the FFT
        fftw_complex* fft_result = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * chunk_size);
        fftw_plan plan = fftw_plan_dft_r2c_1d(chunk_size, NULL, fft_result, FFTW_ESTIMATE);

        // Define the bass range (20Hz to 150Hz)
        int bass_min = 40.0 / sfinfo.samplerate * chunk_size;
        int bass_max = 600.0 / sfinfo.samplerate * chunk_size;

        // Define the mid range (400Hz to 2500Hz)
        int mid_min = 600.0 / sfinfo.samplerate * chunk_size;
        int mid_max = 5000.0 / sfinfo.samplerate * chunk_size;

        // Define the clap range (2000Hz to 4000Hz)
        int clap_min = 5000.0 / sfinfo.samplerate * chunk_size;
        int clap_max = 10000.0 / sfinfo.samplerate * chunk_size;

        std::vector<double> chunk(chunk_size);
        int chunk_number = 0;
        for (int chunkStart = 0; chunkStart < sfinfo.frames; chunkStart += chunk_size) {
            // Read the chunk
            sf_seek(file, chunkStart, SEEK_SET);
            sf_read_double(file, &chunk[0], chunk_size);
            applyWindow(chunk, hammingWindow);
             // Perform the FFT on the current chunk
            fftw_execute_dft_r2c(plan, &chunk[0], fft_result);

            // Check if there's any bass in the current chunk
            double energyBass = 0.0;
            for (int i = bass_min; i <= bass_max; ++i) {
                std::complex<double> value(fft_result[i][0], fft_result[i][1]);
                energyBass += std::norm(value); // square of the absolute value
            }

            // Check if there's any mid in the current chunk
            double energyMid = 0.0;
            for (int i = mid_min; i <= mid_max; ++i) {
                std::complex<double> value(fft_result[i][0], fft_result[i][1]);
                energyMid += std::norm(value); // square of the absolute value
            }

            // Check if there's any clap in the current chunk
            double energyClap = 0.0;
            for (int i = clap_min; i <= clap_max; ++i) {
                std::complex<double> value(fft_result[i][0], fft_result[i][1]);
                energyClap += std::norm(value); // square of the absolute value
            }

            // Calculate the timestamp for the current chunk
            double timestamp = static_cast<double>(chunkStart) / sfinfo.samplerate;

            // Find the average energy levels across all chunks
            double avgEnergyBass = energyBass / chunk_number;
            double avgEnergyMid = energyMid / chunk_number;
            double avgEnergyClap = energyClap / chunk_number;

            // Use these averages to set more dynamic thresholds
            double thresholdBass = avgEnergyBass * 1.5; // Example threshold adjustment
            double thresholdMid = avgEnergyMid * 1.5;
            double thresholdClap = avgEnergyClap * 1.5;

            // Then, in your loop where you determine the sample type
            if (energyBass < thresholdBass && energyMid < thresholdMid && energyClap < thresholdClap) {
                result.at(chunk_number).type = sampleType::SKIP;
            }
            
            else if (energyClap > energyBass && energyClap > energyMid) {
                result.at(chunk_number).type = sampleType::CLAP;
            }
            else if (energyBass > energyMid && energyBass > energyClap) {
                result.at(chunk_number).type = sampleType::BASS;
            }
            
            else if (energyMid > energyBass && energyMid > energyClap) {
                result.at(chunk_number).type = sampleType::MID;
            }

            ++chunk_number;
        }

        // Clean up
        fftw_destroy_plan(plan);
        fftw_free(fft_result);
        sf_close(file);
    }

};
#endif // SONG_ANALIZER_H