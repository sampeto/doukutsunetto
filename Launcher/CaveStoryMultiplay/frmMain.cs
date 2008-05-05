using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CaveStoryMultiplay
{
    public partial class frmMain : Form
    {
        public frmMain()
        {
            InitializeComponent();
        }

        private bool SpawnCaveStoryProcess()
        {
            try
            {
                System.Diagnostics.Process.Start("Doukutsu.exe");
            }
            catch
            {
                return false;
            }
            return true;
        }

        private void btnServer_Click(object sender, EventArgs e)
        {
            if (SpawnCaveStoryProcess())
            {
                Ninjector.Ninjector dllInjector = new Ninjector.Ninjector();
                IntPtr injectionPointer = IntPtr.Zero;

                System.Diagnostics.Process doukutsuProcess = System.Diagnostics.Process.GetProcessesByName("Doukutsu")[0];

                try
                {
                    dllInjector.Inject(doukutsuProcess, "DNServer.dll", out injectionPointer);

                    if (injectionPointer == IntPtr.Zero)    // injection not successful
                    {
                        
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Error starting Cave Story (is Doukutsu.exe in the same directory?)", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnStartClient_Click(object sender, EventArgs e)
        {
            if (SpawnCaveStoryProcess())
            {
                Ninjector.Ninjector dllInjector = new Ninjector.Ninjector();
                IntPtr injectionPointer = IntPtr.Zero;

                System.Diagnostics.Process doukutsuProcess = System.Diagnostics.Process.GetProcessesByName("Doukutsu")[0];

                try
                {
                    dllInjector.Inject(doukutsuProcess, "DNClient.dll", out injectionPointer);

                    if (injectionPointer == IntPtr.Zero)    // injection not successful
                    {

                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Error starting Cave Story (is Doukutsu.exe in the same directory?)", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}