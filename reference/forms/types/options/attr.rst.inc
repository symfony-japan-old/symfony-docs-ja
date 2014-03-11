attr
~~~~

**データ型**: array **デフォルト値**: Empty array

HTML フィールドの表現に追加属性を加えたい場合、\ ``attr`` オプションを使うことが出来ます。これは HTML 属性をキーとした連想配列です。ウィジェットにカスタムクラスを設定する必要があるときに便利です。::

    $builder->add('body', 'textarea', array(
        'attr' => array('class' => 'tinymce'),
    ));

.. 2014/03/07 yositani2002 47553456070b494377c96097c01ec3138ec1090b
