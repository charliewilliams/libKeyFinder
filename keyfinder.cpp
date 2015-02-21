/*************************************************************************

  Copyright 2011-2014 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "keyfinder.h"

namespace KeyFinder {

  key_t KeyFinder::keyOfAudio(const AudioData& originalAudio) {

    Workspace workspace;
    progressiveChromagram(originalAudio, workspace);
    finalChromagram(workspace);

    return keyOfChromaVector(workspace.chromagram->collapseToOneHop());
  }

  void KeyFinder::progressiveChromagram(
    AudioData audio,
    Workspace& workspace
  ) {
    preprocess(audio, workspace);
    workspace.preprocessedBuffer.append(audio);
    chromagramOfBufferedAudio(workspace);
  }

  void KeyFinder::finalChromagram(Workspace& workspace) {
    // flush remainder buffer
    if (workspace.remainderBuffer.getSampleCount() > 0) {
      AudioData flush;
      preprocess(flush, workspace, true);
    }
    // zero padding
    unsigned int paddedHopCount = ceil(workspace.preprocessedBuffer.getSampleCount() / (double)HOPSIZE);
    unsigned int finalSampleLength = FFTFRAMESIZE + ((paddedHopCount - 1) * HOPSIZE);
    workspace.preprocessedBuffer.addToSampleCount(finalSampleLength - workspace.preprocessedBuffer.getSampleCount());
    chromagramOfBufferedAudio(workspace);
  }

  void KeyFinder::preprocess(
    AudioData& workingAudio,
    Workspace& workspace,
    bool flushRemainderBuffer
  ) {

    workingAudio.reduceToMono();

    if (workspace.remainderBuffer.getChannels() > 0) {
      workingAudio.prepend(workspace.remainderBuffer);
      workspace.remainderBuffer.discardFramesFromFront(workspace.remainderBuffer.getFrameCount());
    }

    // TODO: there is presumably some good maths to determine filter frequencies.
    // For now, this approximates original experiment values for default params.
    double lpfCutoff = getLastFrequency() * 1.012;
    double dsCutoff = getLastFrequency() * 1.10;
    unsigned int downsampleFactor = (int) floor(workingAudio.getFrameRate() / 2 / dsCutoff);

    unsigned int bufferExcess = workingAudio.getSampleCount() % downsampleFactor;
    if (!flushRemainderBuffer && bufferExcess != 0) {
      AudioData* remainder = workingAudio.sliceSamplesFromBack(bufferExcess);
      workspace.remainderBuffer.append(*remainder);
      delete remainder;
    }

    // get filter
    const LowPassFilter* lpf = lpfFactory.getLowPassFilter(160, workingAudio.getFrameRate(), lpfCutoff, 2048);
    lpf->filter(workingAudio, workspace, downsampleFactor); // downsampleFactor shortcut
    // note we don't delete the LPF; it's stored in the factory for reuse

    workingAudio.downsample(downsampleFactor);
  }

  void KeyFinder::chromagramOfBufferedAudio(Workspace& workspace) {
    if (workspace.fftAdapter == NULL) {
      workspace.fftAdapter = new FftAdapter(FFTFRAMESIZE);
    }
    SpectrumAnalyser sa(workspace.preprocessedBuffer.getFrameRate(), &ctFactory, &twFactory);
    Chromagram* c = sa.chromagramOfWholeFrames(workspace.preprocessedBuffer, workspace.fftAdapter);
    workspace.preprocessedBuffer.discardFramesFromFront(HOPSIZE * c->getHops());
    if (workspace.chromagram == NULL) {
      workspace.chromagram = c;
    } else {
      workspace.chromagram->append(*c);
      delete c;
    }
  }

  key_t KeyFinder::keyOfChromaVector(const std::vector<double>& chromaVector) const {

    // get key estimate
    KeyClassifier classifier(toneProfileMajor(), toneProfileMinor());
    return classifier.classify(chromaVector);
  }

  key_t KeyFinder::keyOfChromagram(const Workspace& workspace) const {

    // get key estimate
    KeyClassifier classifier(toneProfileMajor(), toneProfileMinor());
    return classifier.classify(workspace.chromagram->collapseToOneHop());
  }

  std::string KeyFinder::stringForKey(const key_t key) const {
        
      switch (key) {
          case A_MAJOR:
              return "A major";
              break;
          case A_MINOR:
              return "A minor";
              break;
          case B_FLAT_MAJOR:
              return "B♭ major";
              break;
          case B_FLAT_MINOR:
              return "B♭ minor";
              break;
          case B_MAJOR:
              return "B major";
              break;
          case B_MINOR:
              return "B minor";
              break;
          case C_MAJOR:
              return "C major";
              break;
          case C_MINOR:
              return "C minor";
              break;
          case D_FLAT_MAJOR:
              return "D♭ major";
              break;
          case D_FLAT_MINOR:
              return "D♭ minor";
              break;
          case D_MAJOR:
              return "D major";
              break;
          case D_MINOR:
              return "D minor";
              break;
          case E_FLAT_MAJOR:
              return "E♭ major";
              break;
          case E_FLAT_MINOR:
              return "E♭ minor";
              break;
          case E_MAJOR:
              return "E major";
              break;
          case E_MINOR:
              return "E minor";
              break;
          case F_MAJOR:
              return "F major";
              break;
          case F_MINOR:
              return "F minor";
              break;
          case G_FLAT_MAJOR:
              return "G♭ major";
              break;
          case G_FLAT_MINOR:
              return "G♭ minor";
              break;
          case G_MAJOR:
              return "G major";
              break;
          case G_MINOR:
              return "G minor";
              break;
          case A_FLAT_MAJOR:
              return "A♭ major";
              break;
          case A_FLAT_MINOR:
              return "A♭ minor";
              break;
          default:
              return "…";
              break;
      }
  }
}
