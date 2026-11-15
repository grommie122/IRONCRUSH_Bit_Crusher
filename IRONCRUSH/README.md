# IRONCRUSH — Windows VST3 Bitcrusher

Industrial-style stereo/mono bitcrusher built with JUCE 8. Seven automatable controls: bit depth (1–16), sample crush (500–44100 Hz), drive (0–30 dB), jitter (0–100%), filter (300–20000 Hz), dry/wet, output (-24–+12 dB). DAW state/presets are saved via JUCE APVTS.

## Build on Windows

Prerequisites: Visual Studio 2022 with **Desktop development with C++**, CMake 3.22+, Git, and internet access (CMake fetches JUCE 8.0.10).

From PowerShell in this folder:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --target IRONCRUSH_VST3 --parallel 4
```

Expected plugin bundle: `build\IRONCRUSH_artefacts\Release\VST3\IRONCRUSH.vst3`.

Copy the entire `.vst3` folder to `C:\Program Files\Common Files\VST3\` and rescan plugins in your DAW. You may need administrator privileges to copy there.

## Build with GitHub Actions

The repository-root workflow at `.github/workflows/main.yml` builds the plugin on GitHub-hosted Windows runners. It runs for pushes and pull requests targeting `main`, and can also be started manually from **Actions → Build Windows VST3 → Run workflow**. Download the `IRONCRUSH-Windows-VST3` artifact after a successful run.

## Signal chain

Input → tanh drive → bit-depth quantization → sample-and-hold rate reduction (optional jitter) → one-pole low-pass → dry/wet → output gain.

## Notes

- A sample-rate knob at or above the DAW sample rate produces no intentional rate reduction.
- The low-pass filter applies only to the wet path.
- This is a first release, not a professionally validated or code-signed binary. Test at low volume before use.
- The source uses JUCE under its licensing terms. Review JUCE's license before distributing proprietary binaries.
