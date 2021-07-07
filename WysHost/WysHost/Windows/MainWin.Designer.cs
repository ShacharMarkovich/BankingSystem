namespace WysHost
{
    partial class MainWin
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWin));
            this.loginBtm = new System.Windows.Forms.Button();
            this.registerBtm = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // loginBtm
            // 
            this.loginBtm.Location = new System.Drawing.Point(39, 50);
            this.loginBtm.Name = "loginBtm";
            this.loginBtm.Size = new System.Drawing.Size(220, 47);
            this.loginBtm.TabIndex = 0;
            this.loginBtm.Text = "Login";
            this.loginBtm.UseVisualStyleBackColor = true;
            this.loginBtm.Click += new System.EventHandler(this.loginBtm_Click);
            // 
            // registerBtm
            // 
            this.registerBtm.Location = new System.Drawing.Point(39, 138);
            this.registerBtm.Name = "registerBtm";
            this.registerBtm.Size = new System.Drawing.Size(220, 47);
            this.registerBtm.TabIndex = 1;
            this.registerBtm.Text = "Register";
            this.registerBtm.UseVisualStyleBackColor = true;
            this.registerBtm.Click += new System.EventHandler(this.registerBtm_Click);
            // 
            // MainWin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(309, 261);
            this.Controls.Add(this.registerBtm);
            this.Controls.Add(this.loginBtm);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainWin";
            this.Text = "Secure Banking Transfer System";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button loginBtm;
        private System.Windows.Forms.Button registerBtm;
    }
}