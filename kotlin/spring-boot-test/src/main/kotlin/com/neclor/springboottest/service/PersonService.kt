package com.neclor.springboottest.service

import com.neclor.springboottest.model.Person
import com.neclor.springboottest.repository.PersonRepository
import org.springframework.stereotype.Service

@Service
class PersonService(private val repo: PersonRepository) {

    fun addAdult(person: Person) {
        if (person.age < 18) {
            throw IllegalArgumentException("Age must be 18 or older")
        }
        repo.addPerson(person)
    }

    fun getAllAdults(): List<Person> = repo.getAllPerson().filter { it.age >= 18 }

    fun findByName(name: String): List<Person> = repo.findByName(name).filter { it.age >= 18 }
}
