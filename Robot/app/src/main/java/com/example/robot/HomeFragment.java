package com.example.robot;

import static android.app.Activity.RESULT_OK;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;

import androidx.core.app.ActivityCompat;
import androidx.fragment.app.Fragment;

import android.speech.RecognizerIntent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.Manifest;
import android.widget.Toast;

import com.example.robot.databinding.FragmentHomeBinding;

import java.util.List;

import okhttp3.WebSocket;

public class HomeFragment extends Fragment {
    private FragmentHomeBinding binding;
    private static final int REQUEST_RECORD_AUDIO_PERMISSION = 200;
    private static final int SPEECH_REQUEST_CODE = 0;

    private boolean permissionToRecordAccepted = false;
    private String[] permissions = {Manifest.permission.RECORD_AUDIO};


    public HomeFragment() {
    }

    public static HomeFragment newInstance(String param1, String param2) {
        HomeFragment fragment = new HomeFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);

        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
        }
        ActivityCompat.requestPermissions(getActivity(), permissions, REQUEST_RECORD_AUDIO_PERMISSION);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        binding = FragmentHomeBinding.inflate(inflater, container, false);

        MainActivity mainActivity = (MainActivity) getActivity();
        WebSocket ws = mainActivity.getWebSocket();

        binding.btnUp.setOnClickListener(v -> {
            ws.send("UP");
            Log.d("WebSocket", "Sent message: UP"); // Log the message sent
        });
        binding.btnDown.setOnClickListener(v -> {
            ws.send("DOWN");
            Log.d("WebSocket", "Sent message: DOWN"); // Log the message sent
        });
        binding.btnLeft.setOnClickListener(v -> {
            ws.send("LEFT");
            Log.d("WebSocket", "Sent message: LEFT"); // Log the message sent
        });
        binding.btnRight.setOnClickListener(v -> {
            ws.send("RIGHT");
            Log.d("WebSocket", "Sent message: RIGHT"); // Log the message sent
        });
        binding.btnPause.setOnClickListener(v -> {
            ws.send("PAUSE");
            Log.d("WebSocket", "Sent message: PAUSE"); // Log the message sent
        });
//        binding.btnDown.setOnClickListener(v -> ws.send("DOWN"));
//        binding.btnLeft.setOnClickListener(v -> ws.send("LEFT"));
//        binding.btnRight.setOnClickListener(v -> ws.send("RIGHT"));
//        binding.btnPause.setOnClickListener(v -> ws.send("PAUSE"));

        binding.btnRecord.setOnClickListener(v -> startVoiceRecognition());

        return binding.getRoot();
    }
    private void startVoiceRecognition() {
        Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
        intent.putExtra(RecognizerIntent.EXTRA_PROMPT, "Speak now...");

        try {
            startActivityForResult(intent, SPEECH_REQUEST_CODE);
        } catch (ActivityNotFoundException a) {
            Toast.makeText(getActivity(), "Your device doesn't support Speech Recognition", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == SPEECH_REQUEST_CODE && resultCode == RESULT_OK) {
            List<String> results = data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
            String spokenText = results.get(0);
            Log.d("WebSocket", "Result: " + spokenText);

            if (spokenText.contains("lên") || spokenText.contains("tiến") || spokenText.contains("tới")) {
                binding.btnUp.performClick();
            }
            else if (spokenText.contains("xuống") || spokenText.contains("lùi") || spokenText.contains("lui")) {
                binding.btnDown.performClick();
            }
            else if (spokenText.contains("trái")) {
                binding.btnLeft.performClick();
            }
            else if (spokenText.contains("phải")) {
                binding.btnRight.performClick();
            }
            else if (spokenText.contains("dừng lại")) {
                binding.btnPause.performClick();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        switch (requestCode) {
            case REQUEST_RECORD_AUDIO_PERMISSION:
                permissionToRecordAccepted = grantResults[0] == PackageManager.PERMISSION_GRANTED;
                break;
        }
        if (!permissionToRecordAccepted) getActivity().finish();
    }

}