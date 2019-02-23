using System;
using System.Data.Entity;
using System.Data.Entity.Infrastructure;

namespace MyStore.Models
{
    public partial class MyStoreEntities : DbContext
    {
        public MyStoreEntities()
            : base("name=MyStoreEntityModelContainer")
        {
        }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            throw new UnintentionalCodeFirstException();
        }

        public virtual DbSet<Category> Categories { get; set; }
        public virtual DbSet<App> Apps { get; set; }
        public virtual DbSet<Developer> Developers { get; set; }
    }
}