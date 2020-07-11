using System;
using System.Threading;
using System.Timers;
using System.Windows;
using System.Windows.Media;
using TungstenOsmosis;
namespace TungstenClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        TungstenWPFInterop tungstenEngine;
        System.Windows.Threading.DispatcherTimer dispatcherTimer;
        Window tungstenWindow;
        MeshEffect meshEffect;
        public MainWindow()
        {
            InitializeComponent();

        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            tungstenEngine.Clear();

            meshEffect.Apply();
            meshEffect.Draw();
            tungstenEngine.RenderOnWindow();
            //Thread.Sleep(1000);
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {

            tungstenEngine.RenderOnWindow();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {



            dispatcherTimer = new System.Windows.Threading.DispatcherTimer(System.Windows.Threading.DispatcherPriority.Send);
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 0, 0, 1);

            tungstenWindow = new TungstenWindow();
            tungstenWindow.Show();

            tungstenEngine = new TungstenWPFInterop();
            //tungstenEngine.InitialiseRendererFromHwnd(NativeWindow.Handle,(int)NativeWindow.ActualWidth,(int)NativeWindow.ActualHeight);
            tungstenEngine.InitialiseRendererFromWindow(tungstenWindow);
            MeshLoader loader = new MeshLoader();
            //loader.Parse("Data//RUST_3d_Low1.obj", "Data//RUST_3d_Low1.bmd");
            TextureLoader textureLoader = new TextureLoader();


            meshEffect = new MeshEffect
            {
                Mesh = loader.Load("Data//cube.bmd"),
                Camera = new Camera(),
                LightSource = new LightSource(),
                Texture = textureLoader.Get("Data//Textures//water.jpg")
            };

            dispatcherTimer.Start();

            DataContext = this;
        }


        public LightSource Light
        {
            get => meshEffect.LightSource;
        }

        public MeshEffect Mesh { get => meshEffect; }
        Color _ambientColor;
        public Color AmbientColor {
            get => _ambientColor;
            set
            {
                _ambientColor = value;
                meshEffect.LightSource.SetAmbient(value.ScR, value.ScG, value.ScB);
            }
        }

        Color _diffuseColor;
        public Color DiffuseColor
        {
            get => _diffuseColor;
            set
            {
                _diffuseColor = value;
                meshEffect.LightSource.SetDiffuse(value.ScR , value.ScG , value.ScB) ;
            }
        }
    }
}
