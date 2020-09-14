#define LIST_FOREACH(var, head, field)                  \
    for((var) = LIST_FIRST(head);                   \
        (var)!= LIST_END(head);                 \
        (var) = LIST_NEXT(var, field))

#define HT_FIND(name, head, elm)     name##_HT_FIND((head), (elm))

 /* Return a pointer to the element in the table 'head' matching 'elm', \
   * or NULL if no such element exists */                               \
  static inline struct type *                                           \
  name##_HT_FIND(const struct name *head, struct type *elm)             \
  {                                                                     \
    struct type **p;                                                    \
    struct name *h = (struct name *) head;                              \
    HT_SET_HASH_(elm, field, hashfn);                                   \
    p = name##_HT_FIND_P_(h, elm);                  \
    return p ? *p : NULL;                                               \
  }      


#define GET_IO_SLOT(x, map, slot, type)                 \
    do {                                \
        struct event_map_entry key_, *ent_;         \
        key_.fd = slot;                     \
        ent_ = HT_FIND(event_io_map, map, &key_);       \
        (x) = ent_ ? &ent_->ent.type : NULL;            \
    } while (0);


void
evmap_io_active_(struct event_base *base, evutil_socket_t fd, short events)
{
    struct event_io_map *io = &base->io;
    struct evmap_io *ctx;
    struct event *ev;

#ifndef EVMAP_USE_HT
    if (fd < 0 || fd >= io->nentries)
        return;
#endif
    GET_IO_SLOT(ctx, io, fd, evmap_io);

    if (NULL == ctx)
        return;
    LIST_FOREACH(ev, &ctx->events, ev_io_next) {
        if (ev->ev_events & events)
            event_active_nolock_(ev, ev->ev_events & events, 1);
    }
}


int main(int argc, char const *argv[])
{
    evmap_io_active_(base, fd, events);
    return 0;
}