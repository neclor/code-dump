package com.neclor.springboottest.repository

import com.neclor.springboottest.model.Person

import org.springframework.stereotype.Repository

@Repository
class PersonRepository {

    private val persons = mutableListOf<Person>()

    fun addPerson(person: Person) = persons.add(person)

    fun getAllPerson() = persons

    fun findByName(name: String): List<Person> = persons.filter { it.name == name }
}
