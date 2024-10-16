#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Base class for waveforms
class Waveform {
protected:
    float frequency; // Frequency in Hz
    float amplitude; // Amplitude
    float phase;     // Phase shift in radians

public:
    virtual ~Waveform() {}

    virtual float generate(float time) = 0; // Generate waveform value at given time
    virtual void setFrequency(float frequency) { this->frequency = frequency; }
    virtual void setAmplitude(float amplitude) { this->amplitude = amplitude; }
    virtual void setPhase(float phase) { this->phase = phase; }
};

// Derived class for Sine wave
class SineWave : public Waveform {
public:
    float generate(float time) override {
        return amplitude * sin(2 * M_PI * frequency * time + phase);
    }
};

// Derived class for Square wave
class SquareWave : public Waveform {
public:
    float generate(float time) override {
        return (sin(2 * M_PI * frequency * time + phase) >= 0) ? amplitude : -amplitude;
    }
};

// Derived class for Triangle wave
class TriangleWave : public Waveform {
public:
    float generate(float time) override {
        float period = 1.0 / frequency;
        float t = fmod(time, period);
        float value = (2.0 * amplitude / period) * t; // Rise
        if (t > period / 2) {
            value = amplitude - (2.0 * amplitude / period) * (t - period / 2); // Fall
        }
        return value;
    }
};

// Function to save waveform data to a CSV file
void saveToCSV(const string& filename, const vector<float>& times, const vector<float>& values) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "Time,Value\n"; // CSV Header
        for (size_t i = 0; i < times.size(); ++i) {
            file << fixed << setprecision(6) << times[i] << "," << values[i] << "\n"; // Write time and value
        }
        file.close();
        cout << "Data saved to " << filename << endl;
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

int main() {
    int choice;
    float frequency, amplitude, phase = 0.0;
    const float duration = 1.0; // Duration in seconds
    const int sampleRate = 1000; // Samples per second
    vector<float> times, values;

    // User input
    cout << "Choose waveform to generate:\n";
    cout << "1. Sine Wave\n2. Square Wave\n3. Triangle Wave\n";
    cout << "Enter your choice (1/2/3): ";
    cin >> choice;

    cout << "Enter frequency (Hz): ";
    cin >> frequency;
    cout << "Enter amplitude: ";
    cin >> amplitude;
    cout << "Enter phase shift (in radians, optional, default is 0): ";
    cin >> phase;

    // Create waveform object
    Waveform* waveform = nullptr;
    switch (choice) {
        case 1: waveform = new SineWave(); break;
        case 2: waveform = new SquareWave(); break;
        case 3: waveform = new TriangleWave(); break;
        default: cout << "Invalid choice!" << endl; return -1;
    }

    // Set parameters
    waveform->setFrequency(frequency);
    waveform->setAmplitude(amplitude);
    waveform->setPhase(phase);

    // Generate waveform data
    for (int i = 0; i <= sampleRate; ++i) {
        float time = i / static_cast<float>(sampleRate); // Calculate time value
        float value = waveform->generate(time); // Generate waveform value
        times.push_back(time);
        values.push_back(value);
    }

    // Save the data to a CSV file
    saveToCSV("waveform_data.csv", times, values);

    // Clean up
    delete waveform;

    return 0;
}
