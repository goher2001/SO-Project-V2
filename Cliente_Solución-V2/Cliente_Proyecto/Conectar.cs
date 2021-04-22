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
    
    public partial class Conectar : Form
    {
        string nombre1;
        public Conectar()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            nombre1 = nombre.Text;
            this.Close();
        }

        private void Conectar_Load(object sender, EventArgs e)
        {
            this.BackColor = Color.LightCoral;
        }
        public string Mensaje()

        {
            string mensaje = "7/" + nombre1 + "/"; 
            return mensaje;

        }
    }
}
