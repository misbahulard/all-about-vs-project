
-- --------------------------------------------------
-- Entity Designer DDL Script for SQL Server 2005, 2008, 2012 and Azure
-- --------------------------------------------------
-- Date Created: 11/03/2018 10:12:11
-- Generated from EDMX file: C:\Users\Misbahul Ardani\source\repos\MyStore\MyStore\MyStoreEntityModel.edmx
-- --------------------------------------------------

SET QUOTED_IDENTIFIER OFF;
GO
USE [MyStore];
GO
IF SCHEMA_ID(N'dbo') IS NULL EXECUTE(N'CREATE SCHEMA [dbo]');
GO

-- --------------------------------------------------
-- Dropping existing FOREIGN KEY constraints
-- --------------------------------------------------


-- --------------------------------------------------
-- Dropping existing tables
-- --------------------------------------------------


-- --------------------------------------------------
-- Creating all tables
-- --------------------------------------------------

-- Creating table 'Categories'
CREATE TABLE [dbo].[Categories] (
    [CategoryId] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL,
    [Description] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Apps'
CREATE TABLE [dbo].[Apps] (
    [AppId] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL,
    [Description] nvarchar(max)  NOT NULL,
    [Price] decimal(18,0)  NOT NULL,
    [DeveloperId] int  NOT NULL,
    [CategoryId] int  NOT NULL,
    [Category_CategoryId] int  NOT NULL,
    [Developer_DeveloperId] int  NOT NULL
);
GO

-- Creating table 'Developers'
CREATE TABLE [dbo].[Developers] (
    [DeveloperId] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL,
    [Description] nvarchar(max)  NOT NULL
);
GO

-- --------------------------------------------------
-- Creating all PRIMARY KEY constraints
-- --------------------------------------------------

-- Creating primary key on [CategoryId] in table 'Categories'
ALTER TABLE [dbo].[Categories]
ADD CONSTRAINT [PK_Categories]
    PRIMARY KEY CLUSTERED ([CategoryId] ASC);
GO

-- Creating primary key on [AppId] in table 'Apps'
ALTER TABLE [dbo].[Apps]
ADD CONSTRAINT [PK_Apps]
    PRIMARY KEY CLUSTERED ([AppId] ASC);
GO

-- Creating primary key on [DeveloperId] in table 'Developers'
ALTER TABLE [dbo].[Developers]
ADD CONSTRAINT [PK_Developers]
    PRIMARY KEY CLUSTERED ([DeveloperId] ASC);
GO

-- --------------------------------------------------
-- Creating all FOREIGN KEY constraints
-- --------------------------------------------------

-- Creating foreign key on [Category_CategoryId] in table 'Apps'
ALTER TABLE [dbo].[Apps]
ADD CONSTRAINT [FK_CategoryApp]
    FOREIGN KEY ([Category_CategoryId])
    REFERENCES [dbo].[Categories]
        ([CategoryId])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_CategoryApp'
CREATE INDEX [IX_FK_CategoryApp]
ON [dbo].[Apps]
    ([Category_CategoryId]);
GO

-- Creating foreign key on [Developer_DeveloperId] in table 'Apps'
ALTER TABLE [dbo].[Apps]
ADD CONSTRAINT [FK_DeveloperApp]
    FOREIGN KEY ([Developer_DeveloperId])
    REFERENCES [dbo].[Developers]
        ([DeveloperId])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_DeveloperApp'
CREATE INDEX [IX_FK_DeveloperApp]
ON [dbo].[Apps]
    ([Developer_DeveloperId]);
GO

-- --------------------------------------------------
-- Script has ended
-- --------------------------------------------------