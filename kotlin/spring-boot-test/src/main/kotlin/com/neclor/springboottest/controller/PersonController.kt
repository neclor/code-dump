package com.neclor.springboottest.controller

import com.neclor.springboottest.model.Person
import com.neclor.springboottest.repository.PersonRepository
import com.neclor.springboottest.service.PersonService

import org.springframework.web.bind.annotation.*

@RestController
@RequestMapping("/api/persons")
class PersonController(private val service: PersonService) {

    @GetMapping("/all")
    fun all(): List<Person> {
        return service.getAllAdults()
    }

    @GetMapping
    fun find(@RequestParam name: String): List<Person> {
        return service.findByName(name)
    }

    @PostMapping
    fun add(@RequestBody person: Person): Person {
        service.addAdult(person)
        return person
    }
}
