// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define GET_CLASSNAME_WITH_FUNCTION		FString(__FUNCTION__)
#define GET_LINE_NUMBER					FString::FromInt(__LINE__)



#define LOG(LogCategory, Message)		UE_LOG(LogCategory, Log, TEXT("%s [%s]: %s"),*GET_CLASSNAME_WITH_FUNCTION, *GET_LINE_NUMBER, *FString(Message))
#define LOG_DISPLAY(LogCategory, Message)		UE_LOG(LogCategory, Display, TEXT("%s [%s]: %s"),*GET_CLASSNAME_WITH_FUNCTION, *GET_LINE_NUMBER, *FString(Message))
#define LOG_VERBOSE(LogCategory, Message)		UE_LOG(LogCategory, Verbose, TEXT("%s [%s]: %s"),*GET_CLASSNAME_WITH_FUNCTION, *GET_LINE_NUMBER, *FString(Message))
#define LOG_VERYVERBOSE(LogCategory, Message)		UE_LOG(LogCategory, VeryVerbose, TEXT("%s [%s]: %s"),*GET_CLASSNAME_WITH_FUNCTION, *GET_LINE_NUMBER, *FString(Message))
#define LOG_WARNING(LogCategory, Message)		UE_LOG(LogCategory, Warning, TEXT("%s [%s]: %s"),*GET_CLASSNAME_WITH_FUNCTION, *GET_LINE_NUMBER, *FString(Message))
#define LOG_ERROR(LogCategory, Message)		UE_LOG(LogCategory, Error, TEXT("%s [%s]: %s"),*GET_CLASSNAME_WITH_FUNCTION, *GET_LINE_NUMBER, *FString(Message))
#define LOG_FATAL(LogCategory, Message)		UE_LOG(LogCategory, Fatal, TEXT("%s [%s]: %s"),*GET_CLASSNAME_WITH_FUNCTION, *GET_LINE_NUMBER, *FString(Message))
