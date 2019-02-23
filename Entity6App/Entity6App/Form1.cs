using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Entity6App
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            //// 6 - 1 -- comment --

            //using (var context = new MyModelContainer())
            //{
            //    var org = new Organizer { Name = "Community Charity" };
            //    var evt = new Event { Name = "Fundraiser" };
            //    org.Events.Add(evt);
            //    context.Organizers.Add(org);
            //    org = new Organizer { Name = "Boy Scouts" };
            //    evt = new Event { Name = "Eagle Scout Dinner" };
            //    org.Events.Add(evt);
            //    context.Organizers.Add(org);
            //    context.SaveChanges();
            //}

            //using (var context = new MyModelContainer())
            //{
            //    var evsorg1 = from ev in context.Events
            //                  from organizer in ev.Organizers
            //                  select new { ev.EventId, organizer.OrganizerId, ev.Name };
            //    Console.WriteLine("Using nested from clauses...");
            //    foreach (var pair in evsorg1)
            //    {
            //        Console.WriteLine("EventId {0}, OrganizerId {1}",
            //        pair.EventId,
            //        pair.OrganizerId);
            //    }
            //    var evsorg2 = context.Events
            //    .SelectMany(e => e.Organizers,
            //    (ev, org) => new { ev.EventId, org.OrganizerId });
            //    Console.WriteLine("\nUsing SelectMany()");
            //    foreach (var pair in evsorg2)
            //    {
            //        Console.WriteLine("EventId {0}, OrganizerId {1}",
            //        pair.EventId, pair.OrganizerId);
            //    }
            //}

            using (var context = new MyModelContainer())
            {
                var worker = new Worker { Name = "Jim" };
                var task = new Task { Title = "Fold Envelopes" };
                var workertask = new WorkerTask { Task = task, Worker = worker };
                context.WorkerTasks.Add(workertask);
                task = new Task { Title = "Mail Letters" };
                workertask = new WorkerTask { Task = task, Worker = worker };
                context.WorkerTasks.Add(workertask);
                worker = new Worker { Name = "Sara" };
                task = new Task { Title = "Buy Envelopes" };
                workertask = new WorkerTask { Task = task, Worker = worker };
                context.WorkerTasks.Add(workertask);
                context.SaveChanges();
            }

            using (var context = new MyModelContainer())
            {
                Console.WriteLine("Workers and Their Tasks");
                Console.WriteLine("=======================");
                foreach (var worker in context.Workers)
                {
                    Console.WriteLine("\n{0}'s tasks:", worker.Name);
                    foreach (var wt in worker.WorkerTasks)
                    {
                        Console.WriteLine("\t{0}", wt.Task.Title);
                    }
                }
            }
        }
    }
}
