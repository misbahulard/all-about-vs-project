using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using MaStore.Models;

namespace MaStore.Controllers
{
    public class CategoryController : Controller
    {
        // GET: Category
        public ActionResult Index()
        {
            using (var db = new Model1Container())
            {
                return View(db.Categories.ToList());
            }
        }

        // GET: Category/Details/5
        public ActionResult Details(int id)
        {
            using (var db = new Model1Container())
            {
                return View(db.Categories.Find(id));
            }
        }

        // GET: Category/Create
        public ActionResult Create()
        {
            return View();
        }

        // POST: Category/Create
        [HttpPost]
        public ActionResult Create(Category categoryValue)
        {
            try
            {
                // TODO: Add insert logic here
                using (var db = new Model1Container())
                {
                    db.Categories.Add(categoryValue);
                    db.SaveChanges();
                }
                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        // GET: Category/Edit/5
        public ActionResult Edit(int id)
        {
            using (var db = new Model1Container())
            {
                return View(db.Categories.Find(id));
            }
        }

        // POST: Category/Edit/5
        [HttpPost]
        public ActionResult Edit(int id, Category categoryValue)
        {
            try
            {
                // TODO: Add update logic here
                using (var db = new Model1Container())
                {
                    db.Entry(categoryValue).State = System.Data.Entity.EntityState.Modified;
                    db.SaveChanges();
                    return RedirectToAction("Index");
                }
            }
            catch
            {
                return View();
            }
        }

        // GET: Category/Delete/5
        public ActionResult Delete(int id)
        {
            using (var db = new Model1Container())
            {
                return View(db.Categories.Find(id));
            }
        }

        // POST: Category/Delete/5
        [HttpPost]
        public ActionResult Delete(int id, Category categoryValue)
        {
            try
            {
                // TODO: Add delete logic here
                using (var db = new Model1Container())
                {
                    db.Entry(categoryValue).State = System.Data.Entity.EntityState.Deleted;
                    db.SaveChanges();
                    return RedirectToAction("Index");
                }
            }
            catch
            {
                return View();
            }
        }
    }
}
