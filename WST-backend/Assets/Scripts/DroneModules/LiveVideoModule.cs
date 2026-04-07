using System;
using System.IO;
using System.Net.Http;
using System.Threading;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

namespace WST.Drone.Modules
{
    public class LiveVideoModule : MonoBehaviour, IDroneModule
    {
        [SerializeField] private RawImage displayImage;
        [SerializeField] private int videoPort = 80;

        private Texture2D _videoTexture;
        private CancellationTokenSource _cancellationTokenSource;
        private HttpClient _httpClient;

        private byte[] _currentFrame;
        private readonly object _frameLock = new object();
        private bool _newFrameReady = false;

        public void Init(DroneManager drone)
        {
            _videoTexture = new Texture2D(2, 2, TextureFormat.RGB24, false);

            if (displayImage != null)
            {
                displayImage.texture = _videoTexture;
                displayImage.enabled = false;
            }

            _httpClient = new HttpClient();
            _httpClient.Timeout = TimeSpan.FromMilliseconds(Timeout.Infinite);
        }

        public void Loop()
        {
            if (_newFrameReady)
            {
                lock (_frameLock)
                {
                    if (_currentFrame != null && _currentFrame.Length > 0)
                    {
                        _videoTexture.LoadImage(_currentFrame);

                        if (displayImage != null && !displayImage.enabled)
                        {
                            displayImage.enabled = true;
                        }
                    }
                    _newFrameReady = false;
                }
            }
        }

        public void ConnectToServer(string url)
        {
            Disconnect();

            string fullUrl = $"http://{url}:{videoPort}";
            _cancellationTokenSource = new CancellationTokenSource();
            Task.Run(() => ReceiveMjpegStream(fullUrl, _cancellationTokenSource.Token));
        }

        public void Disconnect()
        {
            if (_cancellationTokenSource != null)
            {
                _cancellationTokenSource.Cancel();
                _cancellationTokenSource.Dispose();
                _cancellationTokenSource = null;
            }

            if (displayImage != null)
            {
                displayImage.enabled = false;
            }
        }

        private async Task ReceiveMjpegStream(string url, CancellationToken token)
        {
            try
            {
                // ConfigureAwait(false) zapobiega próbom powrotu do głównego wątku Unity
                using (var response = await _httpClient.GetAsync(url, HttpCompletionOption.ResponseHeadersRead, token).ConfigureAwait(false))
                {
                    response.EnsureSuccessStatusCode();

                    using (var stream = await response.Content.ReadAsStreamAsync().ConfigureAwait(false))
                    using (var bufferedStream = new BufferedStream(stream, 65536))
                    {
                        var memoryStream = new MemoryStream();
                        int previousByte = -1;
                        int currentByte;

                        // Ta pętla teraz kręci się bezpiecznie w tle
                        while (!token.IsCancellationRequested && (currentByte = bufferedStream.ReadByte()) != -1)
                        {
                            memoryStream.WriteByte((byte)currentByte);

                            if (previousByte == 0xFF && currentByte == 0xD8)
                            {
                                memoryStream.SetLength(0);
                                memoryStream.WriteByte(0xFF);
                                memoryStream.WriteByte(0xD8);
                            }
                            else if (previousByte == 0xFF && currentByte == 0xD9)
                            {
                                lock (_frameLock)
                                {
                                    _currentFrame = memoryStream.ToArray();
                                    _newFrameReady = true;
                                }
                            }
                            previousByte = currentByte;
                        }
                    }
                }
            }
            catch (OperationCanceledException)
            {
                Debug.Log("[LiveVideoModule] Zakończono połączenie wideo.");
            }
            catch (Exception ex)
            {
                Debug.LogError($"[LiveVideoModule] Błąd strumienia: {ex.Message}");
            }
        }

        private void OnDestroy()
        {
            Disconnect();
            _httpClient?.Dispose();
            if (_videoTexture != null)
            {
                Destroy(_videoTexture);
            }
        }
    }
}