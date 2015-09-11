package com.simprints.scanner.library;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.util.Log;

import java.util.Random;

/**
 * Created by derek on 03/09/2015.
 */
public class AjConnection extends Connection
{
  private int[] devices = new int[1];

  private static final int AUDIO_SAMPLE_FREQUENCY = 44100;
  private AudioRecord audioRecord;
  private int minBufferSize;
  byte[] audioData;

  public AjConnection()
  {
    super("Audio Jack");
  }

  public boolean init() {
    minBufferSize = AudioRecord.getMinBufferSize(AUDIO_SAMPLE_FREQUENCY,AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT);
    audioData = new byte[minBufferSize];
    audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC,AUDIO_SAMPLE_FREQUENCY,AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT,minBufferSize);

    switch (audioRecord.getState())
    {
      case AudioRecord.STATE_INITIALIZED:
        isSetup = true;
        break;
      case AudioRecord.STATE_UNINITIALIZED:
        Log.d("AudioRecord","State is uninitialized");
        break;
    }

    return true;
  }

  public int[] deviceList() {
    return devices;
  }

  public String deviceName(int index)
  {
    return "Audio Jack";
  }

  public String deviceDetail(int index) {
    return "Audio Jack Details";
  }

  public void open(int index) {

  }

  public void close() {

  }

  private int min(int a,int b) {
    return (a<b)? a: b;
  }

  public void read(byte[] r) {
    int nBytes = 0;

    audioRecord.startRecording();
    nBytes = audioRecord.read(audioData,0,audioData.length);

    audioRecord.stop();
    // audioRecord.release();

    System.arraycopy(audioData, 0, r, 0, min(r.length, nBytes));

    Log.d("Audio Jack","read " + nBytes + " bytes");
  }

  public void write(byte[] w) {
    AudioTrack audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,AUDIO_SAMPLE_FREQUENCY,AudioFormat.CHANNEL_OUT_MONO,AudioFormat.ENCODING_PCM_16BIT,minBufferSize,AudioTrack.MODE_STREAM);

    if (audioTrack.getState()==AudioTrack.STATE_INITIALIZED)
    {
      audioTrack.write(audioData,0,audioData.length);
      audioTrack.play();
      audioTrack.stop();
      audioTrack.flush();
      audioTrack.release();
      Log.d("write", "AudioTrack playback");
    }
    else
    {
      Log.d("write", "AudioTrack state is uninitialized");
    }
  }
}
