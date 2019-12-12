CC        := gcc
CFLAGS    := -O3 -Wall
DEPEND    := -MMD
INCLUDES  := include
SRCSDIR   := src
OBJSDIR   := obj
SRCS      := $(foreach dir, $(shell find $(SRCSDIR) -type d), $(wildcard $(dir)/*.c))
OBJS      := $(addprefix $(OBJSDIR)/, $(subst $(SRCSDIR)/,,$(SRCS:.c=.o)))
DEPS      := $(addprefix $(OBJSDIR)/, $(subst $(SRCSDIR)/,,$(SRCS:.c=.d)))
TARGET    := a.out


$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(DEPEND) -o $@ $^ -lm

$(OBJSDIR)/%.o: $(SRCSDIR)/%.c
		@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
		$(CC) $(CFLAGS) $(DEPEND) -I $(INCLUDES) -c $< -o $@

all: $(TARGET)

.PHONY : clean
clean:
		$(RM) -r $(OBJS) $(TARGET)

veryclean:
		$(RM) -r $(OBJSDIR)/* $(TARGET)

-include $(DEPS)

