package com.neclor.springboottest.controller

import com.neclor.springboottest.model.Person
import com.neclor.springboottest.service.PersonService
import org.springframework.stereotype.Controller
import org.springframework.ui.Model
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.PostMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.RequestParam

@Controller
@RequestMapping("/")
class MainController(private val personService: PersonService) {

    @GetMapping
    fun home(
        @RequestParam(required = false) name: String?,
        model: Model
    ): String {
        val persons = if (name.isNullOrBlank()) personService.getAllAdults() else personService.findByName(name)
        model.addAttribute("persons", persons)
        model.addAttribute("search", name ?: "")
        return "home"
    }

    @PostMapping
    fun add(
        @RequestParam personName: String,
        @RequestParam age: Int
    ): String {
        personService.addAdult(Person(personName, age))
        return "redirect:/"
    }
}
