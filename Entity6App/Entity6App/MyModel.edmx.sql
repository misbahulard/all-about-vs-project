
-- --------------------------------------------------
-- Entity Designer DDL Script for SQL Server 2005, 2008, 2012 and Azure
-- --------------------------------------------------
-- Date Created: 11/03/2018 12:39:26
-- Generated from EDMX file: C:\Users\Misbahul Ardani\source\repos\Entity6App\Entity6App\MyModel.edmx
-- --------------------------------------------------

SET QUOTED_IDENTIFIER OFF;
GO
USE [MyDatabase];
GO
IF SCHEMA_ID(N'dbo') IS NULL EXECUTE(N'CREATE SCHEMA [dbo]');
GO

-- --------------------------------------------------
-- Dropping existing FOREIGN KEY constraints
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[FK_EventOrganizer_Event]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[EventOrganizer] DROP CONSTRAINT [FK_EventOrganizer_Event];
GO
IF OBJECT_ID(N'[dbo].[FK_EventOrganizer_Organizer]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[EventOrganizer] DROP CONSTRAINT [FK_EventOrganizer_Organizer];
GO
IF OBJECT_ID(N'[dbo].[FK_WorkerTask_Worker]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[WorkerTask] DROP CONSTRAINT [FK_WorkerTask_Worker];
GO
IF OBJECT_ID(N'[dbo].[FK_WorkerTask_Task]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[WorkerTask] DROP CONSTRAINT [FK_WorkerTask_Task];
GO

-- --------------------------------------------------
-- Dropping existing tables
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[Events]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Events];
GO
IF OBJECT_ID(N'[dbo].[Organizers]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Organizers];
GO
IF OBJECT_ID(N'[dbo].[Workers]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Workers];
GO
IF OBJECT_ID(N'[dbo].[Tasks]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Tasks];
GO
IF OBJECT_ID(N'[dbo].[EventOrganizer]', 'U') IS NOT NULL
    DROP TABLE [dbo].[EventOrganizer];
GO
IF OBJECT_ID(N'[dbo].[WorkerTask]', 'U') IS NOT NULL
    DROP TABLE [dbo].[WorkerTask];
GO

-- --------------------------------------------------
-- Creating all tables
-- --------------------------------------------------

-- Creating table 'Events'
CREATE TABLE [dbo].[Events] (
    [EventId] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Organizers'
CREATE TABLE [dbo].[Organizers] (
    [OrganizerId] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Workers'
CREATE TABLE [dbo].[Workers] (
    [WorkerId] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Tasks'
CREATE TABLE [dbo].[Tasks] (
    [TaskId] int IDENTITY(1,1) NOT NULL,
    [Title] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'EventOrganizer'
CREATE TABLE [dbo].[EventOrganizer] (
    [Events_EventId] int  NOT NULL,
    [Organizers_OrganizerId] int  NOT NULL
);
GO

-- Creating table 'WorkerTask'
CREATE TABLE [dbo].[WorkerTask] (
    [Worker_WorkerId] int  NOT NULL,
    [Tasks_TaskId] int  NOT NULL
);
GO

-- --------------------------------------------------
-- Creating all PRIMARY KEY constraints
-- --------------------------------------------------

-- Creating primary key on [EventId] in table 'Events'
ALTER TABLE [dbo].[Events]
ADD CONSTRAINT [PK_Events]
    PRIMARY KEY CLUSTERED ([EventId] ASC);
GO

-- Creating primary key on [OrganizerId] in table 'Organizers'
ALTER TABLE [dbo].[Organizers]
ADD CONSTRAINT [PK_Organizers]
    PRIMARY KEY CLUSTERED ([OrganizerId] ASC);
GO

-- Creating primary key on [WorkerId] in table 'Workers'
ALTER TABLE [dbo].[Workers]
ADD CONSTRAINT [PK_Workers]
    PRIMARY KEY CLUSTERED ([WorkerId] ASC);
GO

-- Creating primary key on [TaskId] in table 'Tasks'
ALTER TABLE [dbo].[Tasks]
ADD CONSTRAINT [PK_Tasks]
    PRIMARY KEY CLUSTERED ([TaskId] ASC);
GO

-- Creating primary key on [Events_EventId], [Organizers_OrganizerId] in table 'EventOrganizer'
ALTER TABLE [dbo].[EventOrganizer]
ADD CONSTRAINT [PK_EventOrganizer]
    PRIMARY KEY CLUSTERED ([Events_EventId], [Organizers_OrganizerId] ASC);
GO

-- Creating primary key on [Worker_WorkerId], [Tasks_TaskId] in table 'WorkerTask'
ALTER TABLE [dbo].[WorkerTask]
ADD CONSTRAINT [PK_WorkerTask]
    PRIMARY KEY CLUSTERED ([Worker_WorkerId], [Tasks_TaskId] ASC);
GO

-- --------------------------------------------------
-- Creating all FOREIGN KEY constraints
-- --------------------------------------------------

-- Creating foreign key on [Events_EventId] in table 'EventOrganizer'
ALTER TABLE [dbo].[EventOrganizer]
ADD CONSTRAINT [FK_EventOrganizer_Event]
    FOREIGN KEY ([Events_EventId])
    REFERENCES [dbo].[Events]
        ([EventId])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating foreign key on [Organizers_OrganizerId] in table 'EventOrganizer'
ALTER TABLE [dbo].[EventOrganizer]
ADD CONSTRAINT [FK_EventOrganizer_Organizer]
    FOREIGN KEY ([Organizers_OrganizerId])
    REFERENCES [dbo].[Organizers]
        ([OrganizerId])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_EventOrganizer_Organizer'
CREATE INDEX [IX_FK_EventOrganizer_Organizer]
ON [dbo].[EventOrganizer]
    ([Organizers_OrganizerId]);
GO

-- Creating foreign key on [Worker_WorkerId] in table 'WorkerTask'
ALTER TABLE [dbo].[WorkerTask]
ADD CONSTRAINT [FK_WorkerTask_Worker]
    FOREIGN KEY ([Worker_WorkerId])
    REFERENCES [dbo].[Workers]
        ([WorkerId])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating foreign key on [Tasks_TaskId] in table 'WorkerTask'
ALTER TABLE [dbo].[WorkerTask]
ADD CONSTRAINT [FK_WorkerTask_Task]
    FOREIGN KEY ([Tasks_TaskId])
    REFERENCES [dbo].[Tasks]
        ([TaskId])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_WorkerTask_Task'
CREATE INDEX [IX_FK_WorkerTask_Task]
ON [dbo].[WorkerTask]
    ([Tasks_TaskId]);
GO

-- --------------------------------------------------
-- Script has ended
-- --------------------------------------------------