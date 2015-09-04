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
  private AudioTrack audioTrack;
  byte[] audioData;

  public AjConnection()
  {
    super("Audio Jack");
  }

  public boolean init() {
    int minBufferSize = AudioRecord.getMinBufferSize(AUDIO_SAMPLE_FREQUENCY,AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT);
    audioData = new byte[minBufferSize];
    audioRecord = new AudioRecord(MediaRecorder.AudioSource.DEFAULT,AUDIO_SAMPLE_FREQUENCY,AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT,minBufferSize);

    switch (audioRecord.getState())
    {
      case AudioRecord.STATE_INITIALIZED:
        isSetup = true;
        break;
      case AudioRecord.STATE_UNINITIALIZED:

        Log.d("AudioRecord","State is uninitialized");
        break;
    }
    // audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,AUDIO_SAMPLE_FREQUENCY,AudioFormat.CHANNEL_CONFIGURATION_MONO,AudioFormat.ENCODING_PCM_16BIT,minBufferSize,AudioTrack.MODE_STREAM);

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

    for ( int i = 0; i<min(r.length,audioData.length); ++i )
    {
      r[i] = audioData[i];
    }
    Log.d("Audio Jack","read " + r.length + " bytes");
  }

  public void write(byte[] w) {
    // audioTrack.play();

  }
}
