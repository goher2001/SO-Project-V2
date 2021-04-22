using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Cliente_Proyecto
{
    public partial class Registro : Form
    {
        string usuario;
        string contra;
        string contra2;
      
        public Registro()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {if (ContraT2.Text == ContraT.Text)
            {
                usuario = UsuarioT.Text;
                contra = ContraT.Text;
                contra2 = ContraT2.Text;
                this.Close();
            }
        else
            {
                MessageBox.Show("La contraseña no coincide");
            }
        }

        private void Registro_Load(object sender, EventArgs e)
        {
            this.BackColor = Color.LightGreen;
        }
        public string mensaje ()
        {
                string mensaje = "5/" + usuario + "/" + contra;
                return mensaje;
           
        }
    }
}
