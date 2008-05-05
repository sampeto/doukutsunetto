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

            }
            else
            {
                MessageBox.Show("Error starting Cave Story (is Doukutsu.exe in the same directory?)", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
