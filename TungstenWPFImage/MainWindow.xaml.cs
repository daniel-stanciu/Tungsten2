using System;
using System.Windows;
using TungstenOsmosis;

namespace TungstenWPFImage
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        TungstenWPFInterop tungstenEngine;
        System.Windows.Threading.DispatcherTimer dispatcherTimer;
        MeshEffect meshEffect;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 0, 0, 16);

           
           

            tungstenEngine = new TungstenWPFInterop();
            tungstenEngine.InitialiseRendererFromHwnd(NativeWindow.Handle, (int)NativeWindow.ActualWidth, (int)NativeWindow.ActualHeight);

            MeshLoader loader = new MeshLoader();
            loader.Parse("Data//cube.obj", "Data//cube.bmd");
            TextureLoader textureLoader = new TextureLoader();
            var texture = textureLoader.Get("Data//cube.jpg");

            meshEffect = new MeshEffect();
            meshEffect.Mesh = loader.Load("Data//cube.bmd");
            meshEffect.Camera = new Camera();
            meshEffect.LightSource = new LightSource();
            meshEffect.Texture = texture;
            dispatcherTimer.Start();
        }


        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            tungstenEngine.Clear();

            meshEffect.Apply();
            meshEffect.Draw();
            //tungstenEngine.RenderOnImage(testImage);
            tungstenEngine.RenderOnWindow();
        }
    }
}
